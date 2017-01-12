#include <stm32l152x_board.h>
#include "string.h"


struct SystemState DeviceState;
uint32_t HardwareEvents;// = HWE_KEEP_CPU_RUNNING;
uint16_t SoftwareEvents;
uint32_t EventMask;

const uint32_t * const DeviceID = (uint32_t *)0x1FF800D0;

uint8_t UartRxBuffer[32];
uint8_t UartRxCounter;
UartTx Uart2Tx;
uint8_t Uart2Command[sizeof(UartRxBuffer)];

TimerQueue timerSchedule[TIMER_SCHEDULE_SIZE];

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
			uart2SendText("Started by reset");
			setTimer(timer_milliseconds(1000), HWE_LPTIMER);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardRedLedOff();
				setTimer(timer_milliseconds(1000), HWE_LPTIMER);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardGreenLedOff();

				setTimer(timer_seconds(22), HWE_BUTTON_PRESS);						//schedule next wake up

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
			uart2SendText("Woke up by alarm");
			setTimer(timer_milliseconds(1000), HWE_LPTIMER);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardGreenLedOn();
				setTimer(timer_milliseconds(1000), HWE_LPTIMER);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
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
			EnableGps();
			uart2SendText("Woke up by ext event");
			setTimer(timer_milliseconds(1000), HWE_LPTIMER);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardRedLedOn();
				setTimer(timer_milliseconds(1000), HWE_LPTIMER);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
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
			setTimer(timer_seconds(10), HWE_INPUT_TIMEOUT);							//wait for xx seconds before entering low power mode
			HardwareEvents |= HWE_INPUT_WAITING;									//needed as UART RX does not wake this MCU from sleep

			//Here we finished "flashy business" for waking by some event, now let's decide what to do next:
			// 1) let's try getting GPS coordinates and send those once available
			// 2) while GPS is getting ready we can start listening around for control channel availability then
			//    request communication after expected GPS fix delay

//			RadioTask(RTC_START, GPS_FIX_TIMEOUT_WARM);	//can not be longer than 254 seconds
			uint8tmp = GpsTask(GTC_GET_STATE, 0);
			if((GTS_IDLE == uint8tmp) || (GTS_FAILED == uint8tmp)) {
				GpsTask(GTC_RESET, 0);
				GpsTask(GTC_START, 0);												//start mining for GPS data
			}

			uint8tmp = AltimeterTask(ATC_GET_STATE, 0);
			if((ATS_IDLE == uint8tmp) || (ATS_FAILED == uint8tmp)) {
				AltimeterTask(ATC_RESET, 0);
				AltimeterTask(ATC_START, 0);
			}

			uint8tmp = RadioTask(RTC_GET_STATE, 0);
			if((RTS_IDLE == uint8tmp) || (RTS_FAILED == uint8tmp) || (RTS_SUCCESS == uint8tmp)) {
				RadioTask(RTC_RESET, 0);
				RadioTask(RTC_START, timer_seconds(1));								//can not be longer than 254 seconds
			}

			DeviceState.substate = STATE_BUSINESS;
			break;
		default:
//			if(SoftwareEvents & SWE_COMMAND)										//new command received
			if(HardwareEvents & HWE_UART2_COMMAND)
			{
				setTimer(timer_seconds(10), HWE_INPUT_TIMEOUT);						//restart the inactivity timeout
				HardwareEvents |= HWE_INPUT_WAITING;								//keep UART powered for RX
//				SoftwareEvents &= ~SWE_COMMAND;										//command was processed
				CLEAR_EVENT(HWE_UART2_COMMAND);

				startUart2Msg(Uart2Command[0]);										//start the reply with command code

				switch(*Uart2Command)												//parse the command
				{
				case RC_SET_RF_SWITCH:
					driveRfSwitch(Uart2Command[1]);
					break;
				case RC_SET_GREEN_LED:
					driveGreenLed(Uart2Command[1]);
					break;
				case RC_SET_RED_LED:
					driveRedLed(Uart2Command[1]);
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
					LL_RTC_ALMA_ConfigTime(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, Uart2Command[1], Uart2Command[2], Uart2Command[3]);
					LL_RTC_ALMA_SetDay(RTC, Uart2Command[4]);
					LL_RTC_ALMA_SetMask(RTC, (Uart2Command[5] << 24) | (Uart2Command[6] << 16) | (Uart2Command[7] << 8) | Uart2Command[8]);
					LL_RTC_ALMA_Enable(RTC);
//					LL_RTC_EnableWriteProtection(RTC);
					break;
				case RC_GET_DATE:
					addToUart2Msg(LL_RTC_DATE_GetWeekDay(RTC));
					addToUart2Msg(LL_RTC_DATE_GetDay(RTC));
					addToUart2Msg(LL_RTC_DATE_GetMonth(RTC));
					addToUart2Msg(LL_RTC_DATE_GetYear(RTC));
					break;
				case RC_SET_DATE:
					LL_RTC_DisableWriteProtection(RTC);
					LL_RTC_EnableInitMode(RTC);
					while(!LL_RTC_IsActiveFlag_INIT(RTC));
					LL_RTC_DATE_Config(RTC, Uart2Command[1], Uart2Command[2], Uart2Command[3], Uart2Command[4]);
					LL_RTC_DisableInitMode(RTC);
					LL_RTC_EnableWriteProtection(RTC);
					break;
				case RC_GET_TIME:
					addToUart2Msg(LL_RTC_TIME_GetHour(RTC));
					addToUart2Msg(LL_RTC_TIME_GetMinute(RTC));
					addToUart2Msg(LL_RTC_TIME_GetSecond(RTC));
					break;
				case RC_SET_TIME:
					LL_RTC_DisableWriteProtection(RTC);
					LL_RTC_EnableInitMode(RTC);
					while(!LL_RTC_IsActiveFlag_INIT(RTC));
					LL_RTC_TIME_Config(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, Uart2Command[1], Uart2Command[2], Uart2Command[3]);
					LL_RTC_DisableInitMode(RTC);
					LL_RTC_EnableWriteProtection(RTC);
					break;

				case RC_GET_GPS:
					startUart2Msg(RC_TEXT);
					for(uint8tmp = 0; uint8tmp < GpsRxCounter; uint8tmp++)
						addToUart2Msg(GpsRxBuffer[uint8tmp]);
					break;

				default:
					uint8tmp = strlen(Uart2Command);
					spiAccessRegisters(Uart2Command, uint8tmp);
					for(tmp = 1; tmp < uint8tmp; tmp++)
						addToUart2Msg(Uart2Command[tmp]);
				}

				//send back command execution result
				sendUart2Message();
			}

			if(HardwareEvents & HWE_INPUT_TIMEOUT)
			{
				CLEAR_EVENT(HWE_INPUT_TIMEOUT);
				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
				break;


				if((PWR_STANDBY == RadioTask(RTC_GET_PWR_NEED, 0)) &&
					(PWR_STANDBY == GpsTask(GTC_GET_PWR_NEED, 0)) &&
					(PWR_STANDBY == AltimeterTask(ATC_GET_PWR_NEED, 0))) {

					CLEAR_EVENT(HWE_INPUT_WAITING);
					DeviceState.StateAndSubstate = STATE_IDLE;
				}
				else
					setTimer(timer_seconds(10), HWE_INPUT_TIMEOUT);					//restart inactivity timer
			}

			break;	//default substate
		}			//switch substate
		break;		//default state

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

				if((GpsRxBuffer[GpsRxCounter - 1] == '\n') &&						//UartRxCounter can not be less than 1 here
						(GpsRxCounter >= 2)  && (GpsRxBuffer[GpsRxCounter - 2] == '\r'))
				{																	//new command received
					GpsRxBuffer[GpsRxCounter - 2] = 0;								//terminate the string
					HardwareEvents |= HWE_UART1_COMMAND;
					GpsRxCounter = 0;
					break;															//to keep UART1 disabled until message is processed
				}
				if(GpsRxCounter >= sizeof(GpsRxBuffer))								//can not be >= sizeof(UartRxBuffer) as the ISR does not update overflown counter
					GpsRxCounter = 0;

				USARTx_RX_INTERRUPT_ENABLE(USART_1);
			}

			if(HardwareEvents & HWE_UART2_RX_EVENT)
			{
				USARTx_RX_INTERRUPT_DISABLE(USART_2);

				CLEAR_EVENT(HWE_UART2_RX_EVENT);

				if((UartRxBuffer[UartRxCounter - 1] == '\n') &&						//UartRxCounter can not be less than 1 here
						(UartRxCounter >= 2)  && (UartRxBuffer[UartRxCounter - 2] == '\r'))
				{																	//new command received
//					SoftwareEvents |= SWE_COMMAND;
					HardwareEvents |= HWE_UART2_COMMAND;
					memcpy(Uart2Command, UartRxBuffer, UartRxCounter);
					Uart2Command[UartRxCounter - 2] = 0;
					UartRxCounter = 0;
				}

				if(UartRxCounter >= sizeof(UartRxBuffer))							//can not be >= sizeof(UartRxBuffer) as the ISR does not update overflown counter
					UartRxCounter = 0;

				USARTx_RX_INTERRUPT_ENABLE(USART_2);
			}

			//now that current set of events is complete let the state machine process them
			runStateMachine();

			//deal with unprocessed events
			for(EventMask = HWE_UART1_COMMAND; EventMask < HWE_INPUT_WAITING; EventMask <<= 1) {
				switch(HardwareEvents & EventMask) {
				case HWE_GPS_TIMEOUT:
				case HWE_UART1_COMMAND:
					//send command to GPS task
					GpsTask(GTC_EVENT, EventMask);
					//USARTx_RX_INTERRUPT_ENABLE(USART_1);
					break;
				case HWE_UART2_COMMAND:
					uart2SendText("busy");
				case HWE_BUTTON_PRESS:
				case HWE_BUTTON_RELEASE:
					CLEAR_EVENT(EventMask);
					break;
				case HWE_ALARM_A:
					boardRedLedToggle();
					break;
				case HWE_LPTIMER:
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

