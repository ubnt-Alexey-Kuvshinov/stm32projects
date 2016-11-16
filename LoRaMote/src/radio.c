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

uint8_t RadioBuffer[RADIO_BUFFER_SIZE];

RadioSettings radioControlTxSettings = {{0xC0E4, RBW72_7, RSF_12, RCR_4_of_5, REH_EXPLICIT}};
RadioSettings radioDataTxSettings = {};
//RadioSettings radioDataRxSettings = {};


//fq is actual frequency in 61.035Hz steps in reversed order
//void RadioSetCommunicationParameters(uint32_t fq, RadioBW1272 bw, RadioSF sf, RadioCR cr, RadioEH eh)
void RadioSetCommunicationParameters(RadioSettings *rs)
{
	uint32tmp = (0x00 << 8) | RC_SET_MODEM_MODE;						//put device to sleep to set lora mode
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x80 << 8) | RC_SET_MODEM_MODE;						//set LoRa mode bit
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;						//move to standby mode for further programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

//	uint32tmp = (0x00 << 24) | (0xC0 << 16) | (0xE4 << 8) | RC_SET_FREQUENCY;				//set 915MHz
	uint32tmp = (rs->fq << 8) | RC_SET_FREQUENCY;						//frequency in 61.035Hz steps in reversed order
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	uint32tmp = (0x8F << 8) | RC_SET_PA_CONFIG;							//output from PA not RFO pin, use max power
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	uint32tmp = RC_SET_FIFO_POINTER;									//set FIFO pointer, TX and RX base addresses to 00
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	//uint32tmp = (0x60 << 24) | (0xC2 << 16) | (0x0B << 8) | RC_SET_MODEM_CONFIG12_;			//set SF12, CR 4/5, CRC, Explicit header
	uint32tmp = ((rs->sf << 4) | 0x04) << 16;							//SF and  LNA gain set by the internal AGC loop
	uint32tmp |= ((rs->bw << 6) | (rs->cr << 3) | (rs->eh << 2) | 0x03) << 8;	//BW, SF, CR, EH as well as hardcoded payload CRC and LowDataRateOptimize enabled
	uint32tmp |= RC_SET_MODEM_CONFIG12_;
	spiAccessRegisters((uint8_t *)&uint32tmp, 3);

	uint32tmp  = (0x03 << 8) | RC_SET_DETECTION_OPTIMIZE;				//TODO: calculate Detection Optimize depending on BW and SF)
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
}

//RadioSetCommunicationParameters() should be called before to set FIFO pointers and RX parameters
void RadioTransmit(uint8_t *data, uint8_t length)
{
	driveRfSwitch(0x04);												//set to TX from PA
//	uint32tmp = (0x00 << 16) | (0x00 << 8) | RC_SET_FIFO_POINTER;		//set FIFO and TX base addresses to 00

	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;						//move to standby mode for programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	spiAccessRegisters2(RC_SET_FIFO, data, length);						//fill FIFO with the data to transmit
	uint32tmp = (length << 8) | RC_SET_PAYLOAD_LENGTH;					//set message length
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x40 << 8) | RC_SET_DIO_MAPPINGS;						//configure DIO0 for TX done interrupt TODO: touch only DIO0
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x83 << 8) | RC_SET_MODEM_MODE;						//start transmission
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	HardwareEvents |= HWE_RADIO_TRANSMITTING;
}


//void RadioReceive(uint32_t fq, RadioBW1272 bw, RadioSF sf, RadioCR cr, RadioEH eh)
//RadioSetCommunicationParameters() should be called before to set FIFO pointers and TX parameters
void RadioReceive(void)
{
	driveRfSwitch(0x07);												//set to RX
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
//	setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);						//turn the radio chip off, It is the opposite for SX1276!
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

		resetPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);				//turn on the radio chip *It is setPortBit for SX1276!
		setTimer(RADIO_ENABLE_TIMEOUT, HWE_RADIO_TIMEOUT);				//wait until radio chip is ready
		//TODO: increase the delay if needed to satisfy minimum CC request/reply delay
		RadioBuffer[1] = parameter / 1024 + 1;							//express data communication delay in seconds rounded to the greater
		//TODO: get exact time for planned data dump in UTC standard
//		*(RadioBuffer + 4) = RadioBuffer[1] * 1024 - 16;				//express data communication delay in seconds rounded to the greater
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
				RadioBuffer[0] = 0;
				boardRedLedOn();
				setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				state = RTS_FLASHING_CC_REQUEST;
				break;
			case RTS_FLASHING_CC_REQUEST:
			case RTS_FLASHING_DATA_TX:
				boardRedLedToggle();
				if(++RadioBuffer[0] > RADIO_TX_TIMEOUT_BLINKS / 2)
					if(RTS_FLASHING_CC_REQUEST == state) {
						RadioSetCommunicationParameters(&radioControlTxSettings);
						RadioBuffer[0] = RMT_COMMUNICATION_REQUEST;
						RadioTransmit(RadioBuffer, 2);					//RadioBuffer[1] is already set with the delay value
						RadioBuffer[0] = 0;
						setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
						state = RTS_TRANSMITTING_CC_REQUEST;
					} else {
						boardRedLedOff();
						state = RTS_IDLE;
					}
				else
					setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				break;
			case RTS_TRANSMITTING_CC_REQUEST:
			case RTS_TRANSMITTING_DATA:
				if(++RadioBuffer[0] > RADIO_TX_TIMEOUT_BLINKS) {
					boardRedLedOn();
					uart2SendText("radio TX timeout");
					RadioTaskGoToFailed(&state);
				} else {
					boardRedLedToggle();
					setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				}
				break;
			case RTS_LISTENING_CC_REPLY:
			case RTS_LISTENING_DATA_CONFIRMATION:
					uart2SendText("radio RX timeout");
					RadioTaskGoToFailed(&state);
				break;
			case RTS_RECEIVING_CC_REPLY:
			case RTS_RECEIVING_DATA_CONFIRMATION:
				if(++RadioBuffer[0] > RADIO_RX_TIMEOUT_BLINKS) {
					boardRedLedOn();
					uart2SendText("radio RX timeout");
					RadioTaskGoToFailed(&state);
				} else {
					boardGreenLedToggle();
					setTimer(RADIO_LED_TIMEOUT, HWE_RADIO_TIMEOUT);
				}
				break;
			case RTS_WAITING_FOR_DATA_SCHEDULE:
				boardRedLedOn();
				RadioSetCommunicationParameters(&radioDataTxSettings);
				uint8tmp = 0;

				//compose radio message
				RadioBuffer[uint8tmp++] = RMT_COMMUNICATION_DATA;

				if(GTS_SUCCESS == GpsTask(GTC_GET_STATE, 0)) {							//insert GPS data if available
					RadioBuffer[uint8tmp++] = RMDT_GPS;
					memmove(&RadioBuffer[uint8tmp], &GpsData, sizeof(GPS));
					uint8tmp += sizeof(GPS);
				}

				RadioTransmit(RadioBuffer, uint8tmp);									//send the data

				RadioBuffer[0] = 0;														//move to transmitting state
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
//				boardGreenLedOn();
				if(RTS_TRANSMITTING_CC_REQUEST == state) {
					uart2SendText("radio transmitted CC request");
					RadioReceive();
					setTimer(RADIO_RX_TIMEOUT, HWE_RADIO_TIMEOUT);
					state = RTS_LISTENING_CC_REPLY;
				} else {
					uart2SendText("radio transmitted data");
					CLEAR_EVENT(HWE_RADIO_TRANSMITTING);
					CLEAR_EVENT(HWE_RADIO_RECEIVING);
					state = RTS_IDLE;
				}
				break;
			case RTS_RECEIVING_CC_REPLY:								//CC reply received
			case RTS_RECEIVING_DATA_CONFIRMATION:						//TODO: implement
				boardGreenLedOff();

				RadioBuffer[0] = RC_SET_IRQ_FLAGS;						//get and reset all last packet IRQ flags, resetting is important or new interrupt will not trigger unless radio is put to receive mode again(probably after transition to other mode in addition)
				RadioBuffer[1] = 0xFF;
				spiAccessRegisters(RadioBuffer, 2);

				RadioBuffer[6] = RC_GET_RX_BYTES_COUNT;					//get the number of received bytes
				spiAccessRegisters(RadioBuffer + 6, 2);					//RadioBuffer[7] keeps received size

				RadioBuffer[2] = RC_GET_FIFO_RX_LAST_PKT_ADDR;			//get the last received packet start address in FIFO
				spiAccessRegisters(RadioBuffer + 2, 2);					//RadioBuffer[3] keeps last packet address in FIFO
				RadioBuffer[2] = RC_SET_FIFO_POINTER;					//set the FIFO pointer to the beginning of received packet
				spiAccessRegisters(RadioBuffer + 2, 2);

				RadioBuffer[0] = RC_GET_FIFO;							//get the first received byte which is the message type
				spiAccessRegisters(RadioBuffer, 2);

				switch(RadioBuffer[1]) {								//TODO: the texts sending takes time, should be removed
				case RMT_COMMUNICATION_SCHEDULING:
					if(RadioBuffer[7] != 9) {							//TODO: use constant
						uart2SendText("CC reply has wrong size:");
						RadioTaskGoToFailed(&state);
					} else {											//get the channel and time settings
						spiAccessRegisters2(RC_GET_FIFO, radioDataTxSettings.settingsArray, 8);
						setTimer(timer_seconds(radioDataTxSettings.settingsArray[3] - 1), HWE_RADIO_TIMEOUT);	//TODO: implement actual time adjustment based on RX parameters
						state = RTS_WAITING_FOR_DATA_SCHEDULE;			//TODO: implement going to deep sleep wait state logic
						uart2SendText("radio received CC message:");
					}
					break;
				default:
					uart2SendText("CC reply is not recognized:");
					RadioTaskGoToFailed(&state);
				}
				//display received stuff for debugging
				startUart2Msg(RC_RX_MESSAGE);

				RadioBuffer[2] = RC_GET_FIFO_RX_LAST_PKT_ADDR;			//get the last received packet start address in FIFO
				spiAccessRegisters(RadioBuffer + 2, 2);					//RadioBuffer[3] keeps last packet address in FIFO
				RadioBuffer[2] = RC_SET_FIFO_POINTER;					//set the FIFO pointer to the beginning of received packet
				spiAccessRegisters(RadioBuffer + 2, 2);

//				RadioBuffer[0] = Uart2Tx.size;
				spiAccessRegisters2(RC_GET_FIFO, Uart2Tx.data + Uart2Tx.size, RadioBuffer[7]);
				Uart2Tx.size += RadioBuffer[7];

				sendUart2Message();

				RadioBuffer[0] = RC_SET_MODEM_MODE;						//move to standby mode for conserving power
				RadioBuffer[1] = RM_STANDBY;
				spiAccessRegisters(RadioBuffer, 2);


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
			default:
				uart2SendText("radio received unexpected DIO3 event");
			}
			break;
		}		//switch(parameter)
		break;	//case RTC_EVENT:
	default:	//unknown event
		return RTS_FAILED;
	}	//switch(command)

	return RTS_SUCCESS;
}
