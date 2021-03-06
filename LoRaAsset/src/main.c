#include <stm32L051x_board.h>
#include "string.h"


struct SystemState DeviceState;
uint32_t HardwareEvents;// = HWE_KEEP_CPU_RUNNING;
uint16_t SoftwareEvents;
uint32_t EventMask;

const uint32_t * const DeviceID = (uint32_t *)0x1FF80050;

uint8_t Uart1RxBuffer[32];
uint8_t Uart1RxCounter;
TxUart1 Uart1Tx;

uint8_t Uart1Command[sizeof(Uart1RxBuffer)];

uint8_t Uart2RxBuffer[CAMERA_BUFFER_SIZE] __attribute__((aligned(0x04)));
uint8_t Uart2RxCounter;
TxUart2 Uart2Tx;

TimerQueueEntry timerSchedule[TIMER_SCHEDULE_SIZE];
uint16_t TimerCounter;

DeviceDataFields DeviceData;

uint8_t radioTaskRetries;

int32_t tmp;
uint8_t uint8tmp;
uint32_t uint32tmp;


void stateMachine(void)
{
	switch(DeviceState.state) {

	case STATE_STARTING:
		switch(DeviceState.substate)
		{
		case STATE_INIT:
			boardGreenLedOn();
			boardRedLedOn();
			uart1SendText("Started by reset");
			setTimer(timer_milliseconds(1000), HWE_GENERIC_TIMEOUT);
			DeviceState.substate = STATE_LED_CADENCE_1;
//DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_GENERIC_TIMEOUT)
			{
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);
				boardRedLedOff();
				setTimer(timer_milliseconds(1000), HWE_GENERIC_TIMEOUT);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_GENERIC_TIMEOUT)
			{
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);
				boardGreenLedOff();

//				setTimer(timer_seconds(22), HWE_BUTTON_PRESS);						//schedule next wake up

				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
			}
			break;
		}
		break;

	case STATE_ALARM:
		switch(DeviceState.substate)
		{
		case STATE_INIT:
			boardRedLedOn();
			uart1SendText("Woke up by alarm");
			setTimer(timer_milliseconds(1000), HWE_GENERIC_TIMEOUT);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_GENERIC_TIMEOUT)
			{
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);
				boardGreenLedOn();
				setTimer(timer_milliseconds(1000), HWE_GENERIC_TIMEOUT);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_GENERIC_TIMEOUT)
			{
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);
				boardRedLedOff();
				boardGreenLedOff();

				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
			}
			break;
		}
		break;

	case STATE_EXT_EVENT:
		switch(DeviceState.substate)
		{
		case STATE_INIT:
			boardGreenLedOn();
			uart1SendText("Woke up by ext event");
			setTimer(timer_milliseconds(1000), HWE_GENERIC_TIMEOUT);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_GENERIC_TIMEOUT)
			{
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);
				boardRedLedOn();
				setTimer(timer_milliseconds(1000), HWE_GENERIC_TIMEOUT);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_GENERIC_TIMEOUT)
			{
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);
				boardRedLedOff();
				boardGreenLedOff();

				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
			}
			break;
		}
		break;

	case STATE_WAITING_INPUT:
		switch(DeviceState.substate)
		{
		case STATE_INIT:
			uart1SendText("STATE_WAITING_INPUT");
//			setTimer(timer_seconds(10), HWE_INPUT_TIMEOUT);							//wait for xx seconds before entering low power mode
//			HardwareEvents |= HWE_INPUT_WAITING;									//needed as UART RX does not wake this MCU from sleep

			//Here we finished "flashy business" for waking by some event, now let's decide what to do next:
			// 1) let's try getting GPS coordinates and send those once available
			// 2) while GPS is getting ready we can start listening around for control channel availability then
			//    request communication after expected GPS fix delay

//			RadioTask(RTC_START, GPS_FIX_TIMEOUT_WARM);	//can not be longer than 254 seconds


/*
//			uint8tmp = AccelerometerTask(ATC_GET_STATE, 0);
//			if((ATS_IDLE == uint8tmp) || (ATS_FAILED == uint8tmp)) {
				AccelerometerTask(ATC_RESET, 0);
				AccelerometerTask(ATC_START, 0);
//			}
*/

			GpsTask(GTC_RESET, 0);
			GpsTask(GTC_START, 0);													//start mining for GPS data

			AccelerometerTask(ATC_RESET, 0);
			AccelerometerTask(ATC_START, AR_OUT_TEMP_L);							//start temperature reading process
			radioTaskRetries = 1;													//make up to this many attempts to transmit the results over the radio
			setTimer(timer_milliseconds(300), HWE_GENERIC_TIMEOUT);					//set the result check time

			DeviceState.substate = STATE_BUSINESS;
			break;
		default:
			if(HardwareEvents & HWE_UART1_COMMAND)									//new command received
			{
				setTimer(timer_seconds(10), HWE_INPUT_TIMEOUT);						//restart the inactivity timeout
//				HardwareEvents |= HWE_INPUT_WAITING;								//keep UART powered for RX
				CLEAR_EVENT(HWE_UART1_COMMAND);

				startUart1Msg(Uart1Command[1]);										//start the reply with command code

				switch(Uart1Command[1])												//parse the command
				{
				case RC_SET_RF_SWITCH:
					driveRfSwitch(Uart1Command[2]);
					break;
				case RC_SET_GREEN_LED:
					driveGreenLed(Uart1Command[2]);
					break;
				case RC_SET_RED_LED:
					driveRedLed(Uart1Command[2]);
					break;
				case RC_TMP_GREEN_TOGGLE:
					boardGreenLedToggle();
					break;
				case RC_TMP_RED_TOGGLE:
					boardRedLedToggle();
					break;
				case RC_SET_WAKEUP:
					DeviceState.StateAndSubstate = STATE_IDLE;
					CLEAR_EVENT(HWE_KEEP_CPU_RUNNING);
//					LL_RTC_DisableWriteProtection(RTC);
					LL_RTC_ALMA_Disable(RTC);
					RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
					while(!LL_RTC_IsActiveFlag_ALRAW(RTC));
					LL_RTC_ALMA_ConfigTime(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, Uart1Command[2], Uart1Command[3], Uart1Command[4]);
					LL_RTC_ALMA_SetDay(RTC, Uart1Command[5]);
					LL_RTC_ALMA_SetMask(RTC, (Uart1Command[6] << 24) | (Uart1Command[7] << 16) | (Uart1Command[8] << 8) | Uart1Command[9]);
					LL_RTC_ALMA_Enable(RTC);
//					LL_RTC_EnableWriteProtection(RTC);
					break;
				case RC_GET_DATE:
					addToUart1Msg(LL_RTC_DATE_GetWeekDay(RTC));
					addToUart1Msg(LL_RTC_DATE_GetDay(RTC));
					addToUart1Msg(LL_RTC_DATE_GetMonth(RTC));
					addToUart1Msg(LL_RTC_DATE_GetYear(RTC));
					break;
				case RC_SET_DATE:
					LL_RTC_DisableWriteProtection(RTC);
					LL_RTC_EnableInitMode(RTC);
					while(!LL_RTC_IsActiveFlag_INIT(RTC));
					LL_RTC_DATE_Config(RTC, Uart1Command[2], Uart1Command[3], Uart1Command[4], Uart1Command[5]);
					LL_RTC_DisableInitMode(RTC);
					LL_RTC_EnableWriteProtection(RTC);
					break;
				case RC_GET_TIME:
					addToUart1Msg(LL_RTC_TIME_GetHour(RTC));
					addToUart1Msg(LL_RTC_TIME_GetMinute(RTC));
					addToUart1Msg(LL_RTC_TIME_GetSecond(RTC));
					break;
				case RC_SET_TIME:
					LL_RTC_DisableWriteProtection(RTC);
					LL_RTC_EnableInitMode(RTC);
					while(!LL_RTC_IsActiveFlag_INIT(RTC));
					LL_RTC_TIME_Config(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, Uart1Command[2], Uart1Command[3], Uart1Command[4]);
					LL_RTC_DisableInitMode(RTC);
					LL_RTC_EnableWriteProtection(RTC);
					break;
				case RC_GET_TEMP:
					DeviceState.StateAndSubstate = STATE_READING_ACCELEROMETER_TEMPERATURE;
					break;
				case RC_GET_GPS:
					//sendLpUartMessage("$PMTK161,1*29\r\n", 15);
					sendLpUartMessage("$PMTK161,0*28\r\n", 15);
					break;
				case RC_GET_PICTURE:
					CameraTask(CTC_RESET, 0);
					CameraTask(CTC_START, Uart1Command[2]);							//start picture taking process
					DeviceState.StateAndSubstate = STATE_TAKING_PICTURE;
					break;

				default:
					//boardGreenLedToggle();
					spiAccessRegisters(Uart1Command + 1, *Uart1Command);
					for(tmp = 2; tmp <= *Uart1Command; tmp++)
						addToUart1Msg(Uart1Command[tmp]);
				}

				//send back command execution result
				sendUart1Message();
			}

			if(HardwareEvents & HWE_GENERIC_TIMEOUT) {
				boardRedLedToggle();
				CLEAR_EVENT(HWE_GENERIC_TIMEOUT);

				//Let's see what's going on on the board:
				//if any task is in progress - wait for it to finish
				//if all tasks are either idle or finished successfully - start the radio task to report results
				//when all tasks become idle - start inactivity timer
				//when all tasks are idle and inactivity timer expires - go to idle state

				//cancel or retry failed tasks
				if(ATS_FAILED == AccelerometerTask(ATC_GET_STATE, 0))
					AccelerometerTask(ATC_RESET, 0);

				if(GTS_FAILED == GpsTask(GTC_GET_STATE, 0))
					GpsTask(GTC_RESET, 0);

				if((RTS_FAILED == RadioTask(RTC_GET_STATE, 0))|| (RTS_SUCCESS == RadioTask(RTC_GET_STATE, 0)) )
					RadioTask(RTC_RESET, 0);

				//check if all activities have finished
				if((RTS_IDLE == RadioTask(RTC_GET_STATE, 0)) &&
					(GTS_IDLE == GpsTask(GTC_GET_STATE, 0)) &&
					(ATS_IDLE == AccelerometerTask(ATC_GET_STATE, 0))) {

					setTimer(timer_seconds(10), HWE_INPUT_TIMEOUT);					//start inactivity timer
				} else {

				//here we have some uncompleted / unreported activities, let's see if it can be reported over the radio,
				// that is when some task(s) completed successfully, while others are not in progress i.e. all tasks finished
					if( RTS_IDLE == RadioTask(RTC_GET_STATE, 0) ) {
						if(	(ATS_SUCCESS == AccelerometerTask(ATC_GET_STATE, 0) &&
							( GTS_SUCCESS == GpsTask(GTC_GET_STATE, 0) || GTS_IDLE == GpsTask(GTC_GET_STATE, 0))) ||
							(GTS_SUCCESS == GpsTask(GTC_GET_STATE, 0) &&
							( ATS_SUCCESS == AccelerometerTask(ATC_GET_STATE, 0) || ATS_IDLE == AccelerometerTask(ATC_GET_STATE, 0))) ) {
								if(radioTaskRetries) {
									RadioTask(RTC_START, timer_seconds(1));			//can not be longer than 254 seconds
									radioTaskRetries--;
								} else {
									AccelerometerTask(ATC_RESET, 0);
									GpsTask(GTC_RESET, 0);
									RadioTask(RTC_RESET, 0);
								}
							}
					}
					setTimer(timer_milliseconds(300), HWE_GENERIC_TIMEOUT);			//check again after this long
				}
			}


			if(HardwareEvents & HWE_INPUT_TIMEOUT) {
				CLEAR_EVENT(HWE_INPUT_TIMEOUT);

				if((RTS_IDLE == RadioTask(RTC_GET_STATE, 0)) &&
					(GTS_IDLE == GpsTask(GTC_GET_STATE, 0)) &&
					(ATS_IDLE == AccelerometerTask(ATC_GET_STATE, 0))) {

					DeviceState.StateAndSubstate = STATE_IDLE;						//BYE, GOING TO STANDBY
				} else
					setTimer(timer_milliseconds(300), HWE_GENERIC_TIMEOUT);			//check tasks again after this long
			}

			break;	//default substate
		}			//switch substate
		break;		//case STATE_WAITING_INPUT

		case STATE_TAKING_PICTURE:
			switch(DeviceState.substate)
			{
			case STATE_INIT:
				setTimer(timer_milliseconds(330), HWE_INPUT_TIMEOUT);				//restart inactivity timer
				DeviceState.substate = STATE_BUSINESS;
				break;
			default:
				if(HardwareEvents & HWE_INPUT_TIMEOUT)
				{
					CLEAR_EVENT(HWE_INPUT_TIMEOUT);
					boardGreenLedToggle();

					switch(CameraTask(CTC_GET_STATE, 0)) {
					case CTS_FAILED:
						DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
						break;
					case CTS_SUCCESS:
						DeviceState.StateAndSubstate = STATE_TRANSMITTING_PICTURE;
						break;
					default:
						setTimer(timer_milliseconds(330), HWE_INPUT_TIMEOUT);		//keep waiting for task completion
					}
				}

				if(HardwareEvents & HWE_UART1_COMMAND)								//new command received
				{
					CLEAR_EVENT(HWE_UART1_COMMAND);

					switch(Uart1Command[1])	{										//parse the command
					case RC_PICTURE_DATA:
						if(Uart1Command[2])											//resend data
							sendUart1Message();
						else														//get next data
							CameraTask(CTC_GET_NEXT_DATA, 0);
						break;
					}
				}
			}
			break;	//STATE_TAKING_PICTURE

		case STATE_TRANSMITTING_PICTURE:
			switch(DeviceState.substate)
			{
			case STATE_INIT:
				RadioTask(RTC_RESET, 0);											//start RFing picture process
				RadioTask(RTC_START, 0);
				DeviceData.picturePacketNumber = 0;
				DeviceData.pictureRetransmissions = 0;
				setTimer(timer_milliseconds(50), HWE_INPUT_TIMEOUT);				//restart inactivity timer
				DeviceState.substate = STATE_BUSINESS;
				break;
			default:
				if(HardwareEvents & HWE_INPUT_TIMEOUT)
				{
					CLEAR_EVENT(HWE_INPUT_TIMEOUT);
					//boardGreenLedToggle();

					switch(RadioTask(RTC_GET_STATE, 0)) {
					case RTS_SUCCESS:
						if (DDD_PICTURE_NEXT == DeviceData.pictureConfirmation)
							if(DeviceData.picturePacketNumber >= DeviceData.pictureSize) {	//all picture transmitted
								uart1SendText("picture successfully transmitted");
								DeviceData.pictureSize = 0;							//there's no more picture in the device
								DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
								break;
							} else {
								DeviceData.picturePacketNumber++;
								DeviceData.pictureRetransmissions = 0;
							}
					case RTS_FAILED:
					case RTS_IDLE:
						if(DeviceData.pictureRetransmissions++ < 3) {
							RadioTask(RTC_RESET, 0);								//transmit picture packet
							RadioTask(RTC_START, 0);
						} else {
							uart1SendText("picture transmission failed");
							DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
							break;
						}
					default:
						setTimer(timer_milliseconds(50), HWE_INPUT_TIMEOUT);		//restart inactivity timer
					}
				}
			}	//switch(DeviceState.substate)
			break;	//STATE_TRANSMITTING_PICTURE

		case STATE_READING_ACCELEROMETER_TEMPERATURE:
			switch(DeviceState.substate) {
			case STATE_INIT:
				AccelerometerTask(ATC_RESET, 0);
				AccelerometerTask(ATC_START, AR_OUT_TEMP_L);						//start temperature reading process
				setTimer(timer_milliseconds(300), HWE_INPUT_TIMEOUT);				//set task timeout
				DeviceState.substate = STATE_BUSINESS;
				break;
			default:
				if(HardwareEvents & HWE_INPUT_TIMEOUT)
				{
					CLEAR_EVENT(HWE_INPUT_TIMEOUT);

					switch(AccelerometerTask(ATC_GET_STATE, 0)) {
					case ATS_SUCCESS:
						uart1SendText("temperature: ");
						startUart1Msg(RC_GET_TEMP);
						addToUart1Msg(accelerometerData.buffer[1]);
						addToUart1Msg(accelerometerData.buffer[0]);
						sendUart1Message();
						break;
					default:
						uart1SendText("failed reading accelerometer");
						break;
					}

					DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
					break;
				}
			}
			break;	//STATE_READING_TEMPERATURE

		case STATE_IDLE:
			LL_RTC_ALMB_Disable(RTC);
			CLEAR_EVENT(HWE_KEEP_CPU_RUNNING);
			break;
	}
}

//run device state machine until it stabilizes in some state
void runStateMachine(void)
{
	do {
		DeviceState.OldStateAndSubstate = DeviceState.StateAndSubstate;
		stateMachine();
	} while( (DeviceState.StateAndSubstate != DeviceState.OldStateAndSubstate));
}


int main(void)
{
	if(LL_PWR_IsActiveFlag_WU())													//woke up from stand-by mode
	{
		//RTC or external wake-up? Later the states could be loaded from RTC back-up registers.
		if(READ_BIT(RTC->ISR, RTC_ISR_ALRAF))// || READ_BIT(RTC->ISR, RTC_ISR_ALRBF))
			DeviceState.StateAndSubstate = STATE_ALARM;
		else
			DeviceState.StateAndSubstate = STATE_EXT_EVENT;
	}
	else																			//power up or reset
	{
		DeviceState.StateAndSubstate = STATE_STARTING;
	}

	//OldStateAndSubstate and DeviceState.HardwareEvents are zeroed by code initialization

	init_Board_Peripherals();														//Initialize all used peripherals

	//complete initialization actions
	runStateMachine();

	while(1)
	{
		do	//process events
		{
			//first check if UARTs input amounted to something worth processing

			if(HardwareEvents & HWE_UART1_RX_EVENT)
			{
				USARTx_RX_INTERRUPT_DISABLE(USART_1);

				CLEAR_EVENT(HWE_UART1_RX_EVENT);

				if((Uart1RxBuffer[Uart1RxCounter - 1] == '\n') &&					//UartRxCounter can not be less than 1 here
						(Uart1RxCounter >= 2)  && (Uart1RxBuffer[Uart1RxCounter - 2] == '\r'))
				{																	//new command received
					HardwareEvents |= HWE_UART1_COMMAND;
					*Uart1Command = Uart1RxCounter - 2;
					memcpy(Uart1Command + 1, Uart1RxBuffer, *Uart1Command);
					Uart1RxCounter = 0;
				}

				if(Uart1RxCounter >= sizeof(Uart1RxBuffer))							//can not be >= sizeof(UartRxBuffer) as the ISR does not update overflown counter
					Uart1RxCounter = 0;

				USARTx_RX_INTERRUPT_ENABLE(USART_1);
			}

			if(HardwareEvents & HWE_LPUART_RX_EVENT)
			{
				USARTx_RX_INTERRUPT_DISABLE(LPUART);

				CLEAR_EVENT(HWE_LPUART_RX_EVENT);

				if((GpsRxBuffer[GpsRxCounter - 1] == '\n') &&						//UartRxCounter can not be less than 1 here
						(GpsRxCounter >= 2)  && (GpsRxBuffer[GpsRxCounter - 2] == '\r'))
				{																	//new command received
					GpsRxBuffer[GpsRxCounter - 2] = 0;								//terminate the string
					HardwareEvents |= HWE_LPUART_COMMAND;
					GpsRxCounter = 0;
					break;															//to keep LPUART RX disabled until message is processed
				}
				if(GpsRxCounter >= sizeof(GpsRxBuffer))								//can not be >= sizeof(UartRxBuffer) as the ISR does not update overflown counter
					GpsRxCounter = 0;

				USARTx_RX_INTERRUPT_ENABLE(LPUART);
			}

			//now that current set of events is complete let the state machine process them
			runStateMachine();

			//deal with unprocessed events
			for(EventMask = HWE_UART2_RX_EVENT; EventMask < HWE_UART1_TRANSMITTING; EventMask <<= 1) {
				switch(HardwareEvents & EventMask) {
				case HWE_CAMERA_TIMEOUT:
				case HWE_UART2_RX_EVENT:
					CameraTask(CTC_EVENT, EventMask);								//send command to camera task
					break;
				case HWE_GPS_TIMEOUT:
				case HWE_LPUART_COMMAND:
					GpsTask(GTC_EVENT, EventMask);									//send command to GPS task
					//USARTx_RX_INTERRUPT_ENABLE(USART_1);
					break;
				case HWE_UART1_COMMAND:
//					uart1SendText("busy");
				case HWE_BUTTON_PRESS:
				case HWE_BUTTON_RELEASE:
					CLEAR_EVENT(EventMask);
					break;
				case HWE_ALARM_A:
					boardRedLedToggle();
					break;
				case HWE_GENERIC_TIMEOUT:
//					runStateMachine();												//to make sure it did not arrive after runStateMachine() above
					break;
				case HWE_INPUT_TIMEOUT:
//					runStateMachine();
					break;
				case HWE_RADIO_TIMEOUT:
				case HWE_DIO0:
				case HWE_DIO3:
					 RadioTask(RTC_EVENT, EventMask);
					break;
				case HWE_ACCEL_TIMEOUT:
					AccelerometerTask(ATC_EVENT, EventMask);
					break;

				default:
					break;
				}

//				if(HardwareEvents & EventMask)
//					CLEAR_EVENT(EventMask);											//additional switch() can be added to do event-specific clearing
			}
		}
		//while( (DeviceState.StateAndSubstate != DeviceState.OldStateAndSubstate) || HardwareEvents);
		while(HardwareEvents);

		if(DeviceState.StateAndSubstate)											//non-idle state, but currently nothing to do in the system
			enterSleep();
		else
			enterStandby();
	}

}

