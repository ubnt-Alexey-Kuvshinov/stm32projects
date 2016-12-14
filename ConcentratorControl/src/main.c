#include <stm32F103RB_board.h>
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

			startTimer3(1000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
				boardRedLedOff();

				startTimer3(1000);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
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
			startTimer3(2000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
				boardGreenLedOn();

				startTimer3(1000);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
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

			startTimer3(1000);
			DeviceState.substate = STATE_LED_CADENCE_1;
			break;
		case STATE_LED_CADENCE_1:
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
				boardRedLedOn();

				startTimer3(1000);
				DeviceState.substate = STATE_LED_CADENCE_2;
			}
			break;
		case STATE_LED_CADENCE_2:
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
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
			stopTimer3();
			RadioSetCommunicationParameters(0xC0E4, RBW_7, RSF_12, RCR_4_of_5, REH_EXPLICIT);
			RadioReceive();
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
	/*				DeviceState.StateAndSubstate = STATE_IDLE;
					LL_RTC_DisableWriteProtection(RTC_ptr);
					LL_RTC_ALMA_Disable(RTC_ptr);
					RTC_ptr->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC_ptr->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
					while(!LL_RTC_IsActiveFlag_ALRAW(RTC_ptr));
					LL_RTC_ALMA_ConfigTime(RTC_ptr, LL_RTC_ALMA_TIME_FORMAT_AM, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3]);
					LL_RTC_ALMA_SetDay(RTC_ptr, UartRxBuffer[4]);
					LL_RTC_ALMA_SetMask(RTC_ptr, (UartRxBuffer[5] << 24) | (UartRxBuffer[6] << 16) | (UartRxBuffer[7] << 8) | UartRxBuffer[8]);
					LL_RTC_ALMA_Enable(RTC_ptr);
					LL_RTC_EnableWriteProtection(RTC_ptr);
	*/
					uint32tmp = RTC_ptr->CNTH << 16;
					uint32tmp += RTC_ptr->CNTL;
					uint32tmp += 3092;
					enableRtcConfig();
					RTC_ptr->ALRH = uint32tmp >> 16;
					RTC_ptr->ALRL = uint32tmp & 0xFFFF;
					disableRtcConfig();
	//				RTC_ptr->CRH;
					break;
				case RC_GET_DATE:
					addToUart2Msg(RTC_ptr->CNTH & 0xFF);
					addToUart2Msg((RTC_ptr->CNTL >> 8) & 0xFF);
					addToUart2Msg(RTC_ptr->CNTL & 0xFF);
	/*				addToUart2MsgAsHex(LL_RTC_DATE_GetWeekDay(RTC_ptr));
					addToUart2MsgAsHex(LL_RTC_DATE_GetDay(RTC_ptr));
					addToUart2MsgAsHex(LL_RTC_DATE_GetMonth(RTC_ptr));
					addToUart2MsgAsHex(LL_RTC_DATE_GetYear(RTC_ptr));
					break;
				case RC_SET_DATE:
					LL_RTC_DisableWriteProtection(RTC_ptr);
					LL_RTC_EnableInitMode(RTC_ptr);
					while(!LL_RTC_IsActiveFlag_INIT(RTC_ptr));
					LL_RTC_DATE_Config(RTC_ptr, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3], UartRxBuffer[4]);
					LL_RTC_DisableInitMode(RTC_ptr);
					LL_RTC_EnableWriteProtection(RTC_ptr);
					break;
	*/			case RC_GET_TIME:
					addToUart2Msg(RTC_ptr->CNTH & 0xFF);
					addToUart2Msg((RTC_ptr->CNTL >> 8) & 0xFF);
					addToUart2Msg(RTC_ptr->CNTL & 0xFF);
	//				addToUart2MsgAsHex(LL_RTC_TIME_GetMinute(RTC_ptr));
	//				addToUart2MsgAsHex(LL_RTC_TIME_GetSecond(RTC_ptr));
					break;
	/*			case RC_SET_TIME:
					LL_RTC_DisableWriteProtection(RTC_ptr);
					LL_RTC_EnableInitMode(RTC_ptr);
					while(!LL_RTC_IsActiveFlag_INIT(RTC_ptr));
					LL_RTC_TIME_Config(RTC_ptr, LL_RTC_ALMA_TIME_FORMAT_AM, UartRxBuffer[1], UartRxBuffer[2], UartRxBuffer[3]);
					LL_RTC_DisableInitMode(RTC_ptr);
					LL_RTC_EnableWriteProtection(RTC_ptr);
					break;
	*/
				case RC_TX_MESSAGE:
					boardGreenLedToggle();

					RadioTransmit(UartRxBuffer + 1, UartRxCounter - 2 - 1);							//transmit Device ID, msg type, comm parameters and payload

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

			//******************* received radio message ************************************************************************

			if(HardwareEvents & HWE_DIO0)
			{
				CLEAR_EVENT(HWE_DIO0);

//				uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;				//move to standby mode for setting some registers

//				spiAccessRegisters((uint8_t *)&uint32tmp, 2);

				uint32tmp = (0xFF << 8) | RC_SET_IRQ_FLAGS;					//reset all flags but get the previous value, resetting is important or new interrupt will not trigger unless radio is put to receive mode again(probably after transition to other mode in addition)
//				uint32tmp = RC_GET_IRQ_FLAGS;								//get last packet IRQ flags
				spiAccessRegisters((uint8_t *)&uint32tmp, 2);

				//send the received packet with complementary data to concentrator
				startUart2Msg(RC_RX_MESSAGE);								//start the reply with message type

				addToUart2Msg(RTC_ptr->CNTH & 0xFF);						//add time stamp
				addToUart2Msg((RTC_ptr->CNTL >> 8) & 0xFF);
				addToUart2Msg(RTC_ptr->CNTL & 0xFF);

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

//				addToUart2Msg(RC_GET_FIFO);									//read the payload right into Uart TX buffer, and don't forget that RC_GET_FIFO byte is garbage
//				spiAccessRegisters(Uart2Tx.data + Uart2Tx.size - 1, uint32tmp + 1);


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
			startTimer3(7000);										//timeout for transmitting TODO: try use actual time on the air for timeout
			DeviceState.substate = STATE_BUSINESS;
			break;
		default:
			if(HardwareEvents & HWE_DIO0)
			{
				CLEAR_EVENT(HWE_DIO0);
				CLEAR_EVENT(HWE_RADIO_TRANSMITTING);
				DeviceState.StateAndSubstate = STATE_WAITING_INPUT;
			}
			if(HardwareEvents & HWE_TIMER3)
			{
				CLEAR_EVENT(HWE_TIMER3);
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
	DeviceState.StateAndSubstate = STATE_STARTING;

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

			if(HardwareEvents & HWE_TIMER3)
			{
				runStateMachine();								 	//try processing the event
				CLEAR_EVENT(HWE_TIMER3);							//if state machine does not clear it, it's not needed
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


				USARTx_RX_INTERRUPT_ENABLE(USART_2);
			}

			if(HardwareEvents & HWE_BUTTON_PRESS)
			{
				boardGreenLedOn();
				CLEAR_EVENT(HWE_BUTTON_PRESS);
			}

 			if(HardwareEvents & HWE_BUTTON_RELEASE)
			{
				boardGreenLedOff();
				CLEAR_EVENT(HWE_BUTTON_RELEASE);
			}
		}

		while(HardwareEvents);
	}
}

