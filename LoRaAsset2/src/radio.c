/*
 * radio.c
 *
 *  Created on: Oct 28, 2016
 *      Author: Alexey
 */

#include "radio.h"


#define RADIO_BUFFER_SIZE		16
#define RADIO_ENABLE_TIMEOUT	timer_milliseconds(12)					//radio is ready only in 10ms after the Reset input release
#define RADIO_LED_TIMEOUT		timer_milliseconds(400)					//for LED blinking
#define RADIO_TX_TIMEOUT_BLINKS	timer_seconds(2) / RADIO_LED_TIMEOUT	//TX timeout expressed in LED blinks
#define RADIO_RX_TIMEOUT_BLINKS	timer_seconds(2) / RADIO_LED_TIMEOUT	//RX timeout expressed in LED blinks
#define RADIO_TX_TIMEOUT		timer_seconds(2)						//TX timeout expressed in LED blinks
#define RADIO_RX_TIMEOUT		timer_seconds(2)						//RX timeout expressed in LED blinks

uint8_t RadioBuffer[RADIO_BUFFER_SIZE] __attribute__ ((aligned (4)));
//RadioSetCommunicationParameters(0xC0E4, RBW_7, RSF_12, RCR_4_of_5, REH_EXPLICIT);
RadioSettings radioControlTxSettings = {{0xC0E4, RBW_7, RSF_12, RCR_4_of_5, REH_EXPLICIT}};
RadioSettings radioDataTxSettings = {};
//RadioSettings radioDataRxSettings = {};

//fq is actual frequency in 61.035Hz steps in reversed order
void RadioSetCommunicationParameters(RadioSettings *rs)
{
	uint32tmp = (0x00 << 8) | RC_SET_MODEM_MODE;						//put device to sleep to set lora mode
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x80 << 8) | RC_SET_MODEM_MODE;						//set LoRa mode bit
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;						//move to standby mode for further programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (rs->fq << 8) | RC_SET_FREQUENCY;						//frequency in 61.035Hz steps in reversed order
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	uint32tmp = (0xCF << 8) | RC_SET_PA_CONFIG;							//output from PA not RFO pin, use max power
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = RC_SET_FIFO_POINTER;									//set FIFO pointer, TX and RX base addresses to 00
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	uint32tmp = ((rs->sf << 4) | 0x04) << 16;							//SF and RxPayloadCrcOn is ON
	uint32tmp |= ((rs->bw << 4) | (rs->cr << 1) | rs->eh) << 8;			//BW, CR, and  EH
	uint32tmp |= RC_SET_MODEM_CONFIG12_;
	spiAccessRegisters((uint8_t *)&uint32tmp, 3);

	uint32tmp  = (0x0C << 8) | RC_SET_MODEM_CONFIG3;					//LowDataRateOptimize is ON and LNA gain set by the internal AGC loop
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp  = (0x03 << 8) | RC_SET_DETECTION_OPTIMIZE;				//TODO: calculate Detection Optimize depending on BW and SF
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
}

void RadioStartMessage(RadioMessageTypes msgType)
{
	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;						//move to standby mode for programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = RC_SET_FIFO_POINTER;									//set FIFO pointer, TX and RX base addresses to 00
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	uint32tmp = (msgType << 8) | RC_SET_FIFO;							//insert message type
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = *DeviceID;
	spiAccessRegisters2(RC_SET_FIFO, (uint8_t *)&uint32tmp, 4);			//insert Device ID
}

void RadioAddToMessage(uint8_t *data, uint8_t length)
{
	spiAccessRegisters2(RC_SET_FIFO, data, length);						//fill FIFO with the data to transmit
}

//RadioSetCommunicationParameters() should be called before to set FIFO pointers and TX parameters
void RadioTransmit()
{
	driveRfSwitch(0x05);												//set to TX from PA

	uint32tmp = RC_GET_FIFO_POINTER;									//set message length, assuming it starts from 0x00 fifo address
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (uint32tmp & 0xFF00) | RC_SET_PAYLOAD_LENGTH;
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x40 << 8) | RC_SET_DIO_MAPPINGS;						//configure DIO0 for TX done interrupt TODO: touch only DIO0
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

//	uint32tmp = (0xFF << 8) | RC_SET_IRQ_FLAGS;						//get and reset all last packet IRQ flags, resetting is important or new interrupt will not trigger unless radio is put to receive mode again(probably after transition to other mode in addition)
//	spiAccessRegisters(RadioBuffer, 2);

	uint32tmp = (0x83 << 8) | RC_SET_MODEM_MODE;						//start transmission
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	HardwareEvents |= HWE_RADIO_TRANSMITTING;
}

//RadioSetCommunicationParameters() should be called before to set FIFO pointers and TX parameters
void RadioReceive(void)
{
//	driveRfSwitch(0x07);												//set to RX
	driveRfSwitch(0b011);												//set to RX high gain
//	uint32tmp = (0x00 << 16) | (0x00 << 8) | RC_SET_FIFO_POINTER;		//set FIFO and TX base addresses to 00
//	spiAccessRegisters((uint8_t *)&uint32tmp, 3);

	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;						//move to standby mode for programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x01 << 8) | RC_SET_DIO_MAPPINGS;						//configure DIO0 for RX done and DIO3 for Valid header interrupts
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x85 << 8) | RC_SET_MODEM_MODE;						//start continuous reception TODO: use mode constants
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	HardwareEvents |= HWE_RADIO_RECEIVING;
}

static void RadioTaskGoToFailed(RadioTaskStatus *state)
{
	CLEAR_EVENT(HWE_RADIO_TRANSMITTING);
	CLEAR_EVENT(HWE_RADIO_RECEIVING);

//	uint32tmp = (0x80 << 8) | RC_SET_MODEM_MODE;						//move to STOP mode for conserving power
//	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

//	resetPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);					//turn the radio chip off, It is the opposite for SX1276!
	boardRedLedOn();
	*state = RTS_FAILED;
}

uint32_t RadioTask(RadioTaskCommands command, uint32_t parameter)
{
	static RadioTaskStatus state = RTS_IDLE;

	switch(command) {
	case RTC_RESET:														//resets radio task to idle, even if the task is in progress
		state = RTS_IDLE;
		break;
	case RTC_START:														//start new sequence: request transmission parameters from Control Channel - get reply - pack the data at scheduled time - transmit the data using parameters from CC
		if(RTS_IDLE != state)
			return RTS_FAILED;

//		setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);					//turn on the radio chip
		setTimer(RADIO_ENABLE_TIMEOUT, HWE_RADIO_TIMEOUT);				//wait until radio chip is ready
		//TODO: increase the delay if needed to satisfy minimum CC request/reply delay
//		RadioBuffer[1] = parameter / 1024 + 1;							//express data communication delay in seconds rounded to the greater
		state = RTS_ACTIVATING_RADIO;
		break;
	case RTC_GET_STATE:													//request for the task progress/state
		return state;
	case RTC_GET_PWR_NEED:												//request for the task's minimum power requirement to determine power saving mode
		switch (state) {
		case RTS_IDLE:
		case RTS_SUCCESS:
		case RTS_FAILED:
		case RTS_WAITING_FOR_DS_STANDBY:
			return PWR_STANDBY;
		default:
			return PWR_SLEEP;
		}
		break;
	case RTC_EVENT:
		CLEAR_EVENT(parameter);
		switch(parameter) {
		case HWE_RADIO_TIMEOUT:
			switch (state) {
			case RTS_ACTIVATING_RADIO:
				boardRedLedOn();
				RadioSetCommunicationParameters(&radioControlTxSettings);
				RadioStartMessage(RMT_COMMUNICATION_REQUEST);
				RadioTransmit();								//Transmit Communication Request
				setTimer(RADIO_TX_TIMEOUT, HWE_RADIO_TIMEOUT);
				state = RTS_TRANSMITTING_CC_REQUEST;
				break;
			case RTS_TRANSMITTING_CC_REQUEST:
			case RTS_TRANSMITTING_DATA:
				if(++RadioBuffer[0] > RADIO_TX_TIMEOUT_BLINKS) {
					boardRedLedOn();
					lpUartSendText("radio TX timeout");
					RadioTaskGoToFailed(&state);
				} else {
					boardRedLedToggle();
					setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				}
				break;
			case RTS_LISTENING_CC_REPLY:
			case RTS_LISTENING_DATA_CONFIRMATION:
				lpUartSendText("radio RX timeout");
					RadioTaskGoToFailed(&state);
				break;
			case RTS_RECEIVING_CC_REPLY:
			case RTS_RECEIVING_DATA_CONFIRMATION:
				if(++RadioBuffer[0] > RADIO_RX_TIMEOUT_BLINKS) {
					boardRedLedOn();
					lpUartSendText("radio RX timeout");
					RadioTaskGoToFailed(&state);
				} else {
					boardGreenLedToggle();
					setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				}
				break;
			case RTS_WAITING_FOR_DATA_SCHEDULE:
				boardRedLedOn();
				RadioSetCommunicationParameters(&radioDataTxSettings);
//				uint8tmp = 0;

				//pack device data to radio message and air it

				RadioStartMessage(RMT_COMMUNICATION_DATA);

				if(GTS_SUCCESS == GpsTask(GTC_GET_STATE, 0)) {			//insert GPS data if available
					RadioBuffer[0] = RMDT_GPS;
					RadioAddToMessage(RadioBuffer, 1);
					RadioAddToMessage((uint8_t *)&GpsData, sizeof(GPS));
//					RadioBuffer[uint8tmp++] = RMDT_GPS;
//					memmove(&RadioBuffer[uint8tmp], &GpsData, sizeof(GPS));
//					uint8tmp += sizeof(GPS);
					GpsTask(GTC_RESET, 0);
				}

				if(ATS_SUCCESS == AccelerometerTask(ATC_GET_STATE, 0)) {	//insert accelerometer data if available
//					RadioBuffer[uint8tmp++] = RMDT_ACCELEROMETER;
//					RadioBuffer[uint8tmp++] = accelerometerData.temperature_whole;
//					RadioBuffer[uint8tmp++] = accelerometerData.temperature_fractional;
					RadioBuffer[0] = RMDT_ACCELEROMETER;
					RadioBuffer[1] = accelerometerData.temperature_whole;
					RadioBuffer[2] = accelerometerData.temperature_fractional;
					RadioAddToMessage(RadioBuffer, 3);
					AccelerometerTask(ATC_RESET, 0);
				}

				if(DeviceData.pictureSize) {							//insert picture data if available
					RadioBuffer[0] = RMDT_PICTURE;						//TODO: add a function to add single byte by value
					RadioAddToMessage(RadioBuffer, 1);
					RadioAddToMessage((uint8_t *)(0x08008000 + DeviceData.picturePacketNumber * FLASH_PAGE_SIZE), FLASH_PAGE_SIZE);
					DeviceData.pictureConfirmation = DDD_PICTURE_UNCONFIRMED;
				}

				RadioTransmit();										//send the data

				RadioBuffer[0] = 0;										//move to transmitting state
				setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				state = RTS_TRANSMITTING_DATA;
				break;
			}	//switch (state)
			break;	//HWE_RADIO_TIMEOUT event
		case HWE_DIO0:													//interrupt on DIO0 line, can be RX, TX or CAD done event
			switch (state) {
			case RTS_TRANSMITTING_CC_REQUEST:							//transmission finished
			case RTS_TRANSMITTING_DATA:
				boardRedLedOff();
				CLEAR_EVENT(HWE_RADIO_TRANSMITTING);
				if(RTS_TRANSMITTING_CC_REQUEST == state) {
					lpUartSendText("radio transmitted CC request");
					RadioReceive();
					setTimer(RADIO_RX_TIMEOUT, HWE_RADIO_TIMEOUT);
					state = RTS_LISTENING_CC_REPLY;
				} else {
					lpUartSendText("radio transmitted data");
					RadioReceive();
					setTimer(RADIO_RX_TIMEOUT, HWE_RADIO_TIMEOUT);
					state = RTS_RECEIVING_DATA_CONFIRMATION;
				}
				break;
			case RTS_RECEIVING_CC_REPLY:								//CC reply received
			case RTS_RECEIVING_DATA_CONFIRMATION:						//TODO: implement
				boardGreenLedOff();
				CLEAR_EVENT(HWE_RADIO_RECEIVING);

				RadioBuffer[0] = RC_SET_IRQ_FLAGS;						//get and reset all last packet IRQ flags, resetting is important or new interrupt will not trigger unless radio is put to receive mode again(probably after transition to other mode in addition)
				RadioBuffer[1] = 0xFF;
				spiAccessRegisters(RadioBuffer, 2);

				spiAccessRegisters2(RC_GET_RX_BYTES_COUNT, &uint8tmp, 1);//uint8tmp keeps received size

				if(uint8tmp >= RMS_MINIMUM) {							//make sure the message is not too short

					RadioBuffer[2] = RC_GET_FIFO_RX_LAST_PKT_ADDR;		//get the last received packet start address in FIFO
					spiAccessRegisters(RadioBuffer + 2, 2);				//RadioBuffer[3] keeps last packet address in FIFO
					RadioBuffer[2] = RC_SET_FIFO_POINTER;				//set the FIFO pointer to the beginning of received packet
					spiAccessRegisters(RadioBuffer + 2, 2);

//					spiAccessRegisters2(RC_GET_FIFO, RadioBuffer, uint8tmp);	//read the message: Device ID, message type, etc TODO: implement max size check
					spiAccessRegisters2(RC_GET_FIFO, RadioBuffer, RMS_MINIMUM);	//read Device ID and message type

					if(*((int*)RadioBuffer) == *DeviceID) {				//make sure the message is for this device

						//here we have a message intended for this device, lets see what it is
						if(RTS_RECEIVING_CC_REPLY == state) {

							if((RMT_COMMUNICATION_SCHEDULING == RadioBuffer[RMF_MSG_TYPE]) && (RMS_CC_REPLY == uint8tmp)) {
								spiAccessRegisters2(RC_GET_FIFO, radioDataTxSettings.settingsArray, 8);
								setTimer(radioDataTxSettings.settingsArray[3], HWE_RADIO_TIMEOUT);	//TODO: implement actual time adjustment based on RX parameters
								state = RTS_WAITING_FOR_DATA_SCHEDULE;			//TODO: implement going to deep sleep wait state logic
								lpUartSendText("radio received CC message:");
								goto gotit;
							}
						} else if(RMT_COMMUNICATION_DATA_CONFIRMATION == RadioBuffer[RMF_MSG_TYPE]) {	//radio task is in RTS_RECEIVING_DATA_CONFIRMATION state
							int i;

							//uint8tmp -= RMS_MINIMUM;
							spiAccessRegisters2(RC_GET_FIFO, RadioBuffer, uint8tmp - RMS_MINIMUM);	//get data confirmation message itself

							for (i = 0; i < uint8tmp - RMS_MINIMUM; )
								switch(RadioBuffer[i]) {					//TODO: implement size checks
								case RMDT_PICTURE:
									DeviceData.pictureConfirmation = RadioBuffer[i + 1];
									i += 2;
									break;
								default:
									i = uint8tmp;
								}

							state = RTS_SUCCESS;
							lpUartSendText("radio received Data message:");
							goto gotit;
						}
					}
				}

				lpUartSendText("CC or data reply is not recognized:");
				RadioTaskGoToFailed(&state);							//TODO: may try receiving again
gotit:
				startLpUartMsg(RC_RX_MESSAGE);							//display received stuff for debugging

				RadioBuffer[2] = RC_GET_FIFO_RX_LAST_PKT_ADDR;			//get the last received packet start address in FIFO
				spiAccessRegisters(RadioBuffer + 2, 2);					//RadioBuffer[3] keeps last packet address in FIFO
				RadioBuffer[2] = RC_SET_FIFO_POINTER;					//set the FIFO pointer to the beginning of received packet
				spiAccessRegisters(RadioBuffer + 2, 2);

				spiAccessRegisters2(RC_GET_FIFO, LpUartTx.data + LpUartTx.size, uint8tmp);
				LpUartTx.size += uint8tmp;

				sendLpUartMessage();

//				RadioBuffer[0] = RC_SET_MODEM_MODE;						//move to standby mode for conserving power
//				RadioBuffer[1] = RM_STANDBY;
//				spiAccessRegisters(RadioBuffer, 2);
				uint32tmp = (0x00 << 8) | RC_SET_MODEM_MODE;			//move to STOP mode for conserving power
				spiAccessRegisters((uint8_t *)&uint32tmp, 2);
				break;
			}		//switch (state)
			break;	//case HWE_DIO0
		case HWE_DIO3:													//interrupt on DIO3 line, can be reception of a valid header or PayloadCrcError
			switch (state) {
			case RTS_LISTENING_CC_REPLY:
				boardGreenLedToggle();
				setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				RadioBuffer[0] = 0;
				state = RTS_RECEIVING_CC_REPLY;
				break;
			case RTS_RECEIVING_DATA_CONFIRMATION:
				boardGreenLedToggle();
				break;
			default:
				lpUartSendText("radio received unexpected DIO3 event");
			}
			break;
		}		//switch(parameter)
		break;	//case RTC_EVENT:
	default:	//unknown event
		return RTS_FAILED;
	}	//switch(command)

	return RTS_SUCCESS;
}
