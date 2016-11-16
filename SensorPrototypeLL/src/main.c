#include "stm32L05x_board.h"
#include "string.h"


#define CLEAR_EVENT(event) __disable_irq(); HardwareEvents &= ~event; __enable_irq()



struct SystemState DeviceState;
uint16_t HardwareEvents;
uint16_t SoftwareEvents;

uint8_t UartRxBuffer[32];
uint8_t UartRxCounter;

UartTx Uart2Tx;

int32_t tmp;
uint8_t uint8tmp;


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
			startLpTimer(9000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardRedLedOff();
				startLpTimer(9000);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardGreenLedOff();
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
			startLpTimer(1000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardGreenLedOn();
				startLpTimer(1000);
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
			uart2SendText("Woke up by ext event");
			startLpTimer(1000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardRedLedOn();
				startLpTimer(1000);
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
		if(SoftwareEvents & SWE_COMMAND)					//new command received
		{
			SoftwareEvents &= ~SWE_COMMAND;					//command was processed
			while(HardwareEvents & HWE_UART2_TRANSMITTING);	//wait until previous message, if any, finished transmitting
			startUart2Msg(UartRxBuffer[0]);					//start the reply with command code

			switch(*UartRxBuffer)							//parse the command
			{
			case RC_SET_RF_SWITCH:
				driveRfSwitch(UartRxBuffer[1]);
				break;
			case RC_SET_GREEN_LED:
				driveGreenLed(UartRxBuffer[1]);
				break;
			case RC_SET_RED_LED:
				driveRedLed(UartRxBuffer[1]);
				break;
			case RC_TMP_GREEN_TOGGLE:
				boardGreenLedToggle();
				break;
			case RC_TMP_RED_TOGGLE:
				boardRedLedToggle();
				break;
			case RC_SET_WAKEUP:
				DeviceState.StateAndSubstate = STATE_IDLE;
				LL_RTC_DisableWriteProtection(RTC);
				LL_RTC_ALMA_Disable(RTC);
				RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
				while(!LL_RTC_IsActiveFlag_ALRAW(RTC));
				LL_RTC_ALMA_ConfigTime(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3]);
				LL_RTC_ALMA_SetDay(RTC, UartRxBuffer[4]);
				LL_RTC_ALMA_SetMask(RTC, (UartRxBuffer[5] << 24) | (UartRxBuffer[6] << 16) | (UartRxBuffer[7] << 8) | UartRxBuffer[8]);
				LL_RTC_ALMA_Enable(RTC);
				LL_RTC_EnableWriteProtection(RTC);
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
				LL_RTC_DATE_Config(RTC, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3], UartRxBuffer[4]);
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
				LL_RTC_TIME_Config(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3]);
				LL_RTC_DisableInitMode(RTC);
				LL_RTC_EnableWriteProtection(RTC);
				break;
			default:
				spiAccessRegisters(UartRxBuffer, UartRxCounter - 2);
				for(tmp = 1; tmp < UartRxCounter - 2; tmp++)
					addToUart2Msg(UartRxBuffer[tmp]);
			}

			//send back command execution result
			sendUart2Message();
		}
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
	if(LL_PWR_IsActiveFlag_WU())									//woke up from stand-by mode
	{
		//RTC or external wake-up? Later the states could be loaded from RTC back-up registers.
		if(READ_BIT(RTC->ISR, RTC_ISR_ALRAF))	//or (READ_BIT(RTC->ISR, RTC_ISR_ALRBF));
			DeviceState.StateAndSubstate = STATE_ALARM;
		else
			DeviceState.StateAndSubstate = STATE_EXT_EVENT;
	}
	else															//power up or reset
	{
		DeviceState.StateAndSubstate = STATE_STARTING;
	}

	//OldStateAndSubstate and DeviceState.HardwareEvents are zeroed by code initialization

	init_Board_Peripherals();										//Initialize all used peripherals

	//complete initialization actions
	runStateMachine();

	while(1)
	{
		do
		{
			//process hardware events
			if(HardwareEvents & HWE_ALARM_A)
			{
				boardRedLedToggle();
				CLEAR_EVENT(HWE_ALARM_A);
			}

			if(HardwareEvents & HWE_LPTIMER)
			{
				runStateMachine();								 	//try processing the event
				CLEAR_EVENT(HWE_LPTIMER);							//if state machine does not clear it, it's not needed
			}

			if(HardwareEvents & HWE_UART2_RX_EVENT)
			{
//				NVIC_DisableIRQ(USART2_IRQn);						//make sure first character of new command would not get lost while parsing
//				USARTx_RX_DISABLE(USART_2);							//disable UART RX until command is processed and RX re-enabled
				USARTx_RX_INTERRUPT_DISABLE(USART_2);

				CLEAR_EVENT(HWE_UART2_RX_EVENT);

				if((UartRxBuffer[UartRxCounter - 1] == '\n') &&		//UartRxCounter can not be less than 1 here
						(UartRxCounter >= 2)  && (UartRxBuffer[UartRxCounter - 2] == '\r'))
				{													//new command received
					SoftwareEvents |= SWE_COMMAND;

					runStateMachine();								//try processing the command

					if(SoftwareEvents & SWE_COMMAND)				//check if the message was processed
					{
						SoftwareEvents &= ~SWE_COMMAND;				//drop this command, alternatively it can be saved for later if needed
						uart2SendText("busy");
					}
					UartRxCounter = 0;								//reset for next command
				}

				if(UartRxCounter >= sizeof(UartRxBuffer))			//can not be >= sizeof(UartRxBuffer) as the ISR does not update overflown counter
					UartRxCounter = 0;

				//USARTx_RX_ENABLE(USART_2);
				USARTx_RX_INTERRUPT_ENABLE(USART_2);
			}

			if(HardwareEvents & HWE_BUTTON_PRESS)
			{
//				boardGreenLedOn();
				CLEAR_EVENT(HWE_BUTTON_PRESS);
			}

			if(HardwareEvents & HWE_BUTTON_RELEASE)
			{
//				boardGreenLedOff();
				CLEAR_EVENT(HWE_BUTTON_RELEASE);
			}
		}
		//while( (DeviceState.StateAndSubstate != DeviceState.OldStateAndSubstate) || HardwareEvents);
		while(HardwareEvents);

		if(DeviceState.StateAndSubstate)							//non-idle state, but currently nothing to do in the system
			enterSleep();
		else
			enterStandby();
	}

}

