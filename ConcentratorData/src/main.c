#include "stm32L05x_board.h"
#include "string.h"


#define CLEAR_EVENT(event) __disable_irq(); HardwareEvents &= ~event; __enable_irq()



struct SystemState DeviceState;
uint16_t HardwareEvents;
uint16_t SoftwareEvents;

uint8_t UartRxBuffer[256 + 16];
uint16_t UartRxCounter;

UartTx Uart2Tx;

int32_t tmp;
uint8_t uint8tmp;
uint32_t uint32tmp;

//RadioSettings radioControlTxSettings = {{0xC0E4, RBW72_7, RSF_12, RCR_4_of_5, REH_EXPLICIT}};
RadioSettings radioDataTxSettings = {};
RadioSettings radioDataRxSettings = {};

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
			startLpTimer(1000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				boardRedLedOff();
				startLpTimer(1000);
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
	
		switch(DeviceState.substate)
		{
		case STATE_INIT:
			stopLpTimer();

//			RadioSetCommunicationParameters(0xC0E4, RBW_7, RSF_12, RCR_4_of_5, REH_EXPLICIT);
//			RadioReceive();
			DeviceState.substate = STATE_BUSINESS;
			break;
		default:
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
/*					DeviceState.StateAndSubstate = STATE_IDLE;
					LL_RTC_DisableWriteProtection(RTC);
					LL_RTC_ALMA_Disable(RTC);
					RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
					while(!LL_RTC_IsActiveFlag_ALRAW(RTC));
					LL_RTC_ALMA_ConfigTime(RTC, LL_RTC_ALMA_TIME_FORMAT_AM, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3]);
					LL_RTC_ALMA_SetDay(RTC, UartRxBuffer[4]);
					LL_RTC_ALMA_SetMask(RTC, (UartRxBuffer[5] << 24) | (UartRxBuffer[6] << 16) | (UartRxBuffer[7] << 8) | UartRxBuffer[8]);
					LL_RTC_ALMA_Enable(RTC);
					LL_RTC_EnableWriteProtection(RTC);
*/					break;
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

				case RC_RX_MESSAGE:
					boardGreenLedOn();
					memcpy(radioDataRxSettings.settingsArray, UartRxBuffer + 2, 8);
					RadioSetCommunicationParameters(radioDataRxSettings.settingsArray);

					RadioReceive();

					break;

				case RC_TX_MESSAGE:
					boardGreenLedToggle();

					uint8tmp = RMT_COMMUNICATION_REQUEST;

					//RadioSetCommunicationParameters(0xC0E4, RBW_7, RSF_12, RCR_4_of_5, REH_EXPLICIT);
//					RadioSetCommunicationParameters(
//							(UartRxBuffer[1] << 16) | (UartRxBuffer[2] << 8) | UartRxBuffer[3],		//frequency in 61.035Hz increments in reversed order
//							UartRxBuffer[4], UartRxBuffer[5], UartRxBuffer[6],	UartRxBuffer[7]);	//BW, SF, CR, and header type

//					RadioTransmit(UartRxBuffer + 8, UartRxCounter - 2 - 8);							//transmit the payload
					RadioTransmit(UartRxBuffer + 1, UartRxCounter - 2 - 1);							//transmit msg type, comm parameters and payload

					DeviceState.StateAndSubstate = STATE_RADIO_TRANSMITTING;
					break;
				default:
					spiAccessRegisters(UartRxBuffer, UartRxCounter - 2);
					for(tmp = 1; tmp < UartRxCounter - 2; tmp++)
						addToUart2Msg(UartRxBuffer[tmp]);
				}	//switch(command)

				//send back command execution result
				sendUart2Message();
			}	//if(SoftwareEvents & SWE_COMMAND)

			if(HardwareEvents & HWE_DIO0)
			{
				CLEAR_EVENT(HWE_DIO0);
				boardGreenLedOff();

				uint32tmp = (0xFF << 8) | RC_SET_IRQ_FLAGS;					//reset all flags but get the previous value, resetting is important or new interrupt will not trigger unless radio is put to receive mode again(probably after transition to other mode in addition)
				spiAccessRegisters((uint8_t *)&uint32tmp, 2);

				//send the received packet with complementary data to concentrator
				startUart2Msg(RC_RX_MESSAGE);								//start the reply with message type

				addToUart2Msg(LL_RTC_TIME_GetHour(RTC));					//add time stamp
				addToUart2Msg(LL_RTC_TIME_GetMinute(RTC));
				addToUart2Msg(LL_RTC_TIME_GetSecond(RTC));

				addToUart2Msg(uint32tmp >> 8);								//add IRQ flags byte

				uint32tmp = RC_GET_PACKET_SNR;								//add packet's SNR and RSSI
				spiAccessRegisters((uint8_t *)&uint32tmp, 3);
				addToUart2Msg(uint32tmp >> 8);
				addToUart2Msg(uint32tmp >> 16);

				uint32tmp = RC_GET_FIFO_RX_LAST_PKT_ADDR;					//get the last received packet start address in FIFO
				spiAccessRegisters((uint8_t *)&uint32tmp, 2);
				uint32tmp = (uint32tmp & 0xFF00) | RC_SET_FIFO_POINTER;		//set the FIFO pointer to the beginning of received packet
				spiAccessRegisters((uint8_t *)&uint32tmp, 2);

				uint32tmp = RC_GET_RX_BYTES_COUNT;							//get the number of received bytes
				spiAccessRegisters((uint8_t *)&uint32tmp, 2);
				uint32tmp >>= 8;

				//read the payload right into Uart TX buffer
				spiAccessRegisters2(RC_GET_FIFO, Uart2Tx.data + Uart2Tx.size, uint32tmp);
				Uart2Tx.size += uint32tmp;

				sendUart2Message();

			}
		}		//switch sub state of STATE_WAITING_INPUT state
		break;	//case STATE_WAITING_INPUT

	case STATE_RADIO_TRANSMITTING:
		switch(DeviceState.substate)
		{
		case STATE_INIT:
			boardRedLedOn();
			startLpTimer(7000);										//timeout for transmitting TODO: try use actual time on the air for timeout
			DeviceState.substate = STATE_BUSINESS;
			break;
		default:
			if(HardwareEvents & HWE_DIO0)
			{
				CLEAR_EVENT(HWE_DIO0);
				CLEAR_EVENT(HWE_RADIO_TRANSMITTING);
				boardRedLedOff();
				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
			}
			if(HardwareEvents & HWE_LPTIMER)
			{
				CLEAR_EVENT(HWE_LPTIMER);
				CLEAR_EVENT(HWE_RADIO_TRANSMITTING);
				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
//				DeviceState.state = STATE_WAITING_INPUT;
//				DeviceState.substate = STATE_BUSINESS;
				boardRedLedOn();
				uart2SendText("radio transmitting timeout");
			}
		}
		break;

	}	//switch(state)
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

			if(HardwareEvents & HWE_DIO0)
			{
				runStateMachine();								 	//SM should clear it, TODO: run the SM for all at once
			}

			if(HardwareEvents & HWE_UART2_RX_EVENT)
			{
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
	}
}

