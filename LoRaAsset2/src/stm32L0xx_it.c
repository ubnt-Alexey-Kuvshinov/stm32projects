#include "stm32L052x_board.h"

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

//	.word	NMI_Handler
//	.word	HardFault_Handler
//	.word	MemManage_Handler
//	.word	BusFault_Handler
//	.word	UsageFault_Handler

//	.word	SVC_Handler
//	.word	DebugMon_Handler
//	.word	PendSV_Handler
//	.word	SysTick_Handler

void NMI_Handler(void)
{
	while (1)
		boardRedLedToggle();
}

void HardFault_Handler(void) {
	while (1)
		boardRedLedToggle();
}

void MemManage_Handler(void) {
	while (1)
		boardRedLedToggle();
}

void BusFault_Handler(void)
{
	while (1)
		boardRedLedToggle();
}

void UsageFault_Handler(void)
{
	while (1)
		boardRedLedToggle();
}

void SVC_Handler(void)
{
	while (1)
		boardRedLedToggle();
}


void DebugMon_Handler(void)
{
	while (1)
		boardRedLedToggle();
}


void PendSV_Handler(void)
{
	while (1)
		boardRedLedToggle();
}


void SysTick_Handler(void)
{
	while (1)
		boardRedLedToggle();


}

void WWDG_IRQHandler(void)
{
	  while (1)
	  {
		  boardRedLedToggle();
	  }
}



void EXTI0_1_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF0) {													//movement interrupt from accelerometer
		EXTI->PR = (EXTI_PR_PIF0);
	}
}

void EXTI2_3_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF2) {													//DIO0 from the radio
		EXTI->PR = EXTI_PR_PIF2;
		HardwareEvents |= HWE_DIO0;
	}
}

void EXTI4_15_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF5) {													//DIO3 from the radio
		EXTI->PR = EXTI_PR_PIF5;
		HardwareEvents |= HWE_DIO3;
	}
}


void TIM6_DAC_IRQHandler(void)
{
//  HAL_TIM_IRQHandler(&TimHandle);
}


void USART1_IRQHandler(void)
{
	if((USART_ptr(USART_1)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(USART_1)->ISR & USART_ISR_RXNE)) {			//new byte received
		if(GpsRxCounter < sizeof(GpsRxBuffer))
			GpsRxBuffer[GpsRxCounter++] = USART_ptr(USART_1)->RDR;
		else
			USART_ptr(USART_1)->RQR |= USART_RQR_RXFRQ;								//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_UART1_RX_EVENT;
	} else if((USART_ptr(USART_1)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(USART_1)->ISR & USART_ISR_ORE)) {		//overrun error occurred																//then it was triggered because of overrun error
		USART_ptr(USART_1)->ICR |= USART_ICR_ORECF;									//clear overrun flag
		//TODO: mark the message as invalid
	} else if((USART_ptr(USART_1)->CR1 | USART_CR1_TCIE) && (USART_ptr(USART_1)->ISR & USART_ISR_TC)) {			//byte transmission finished
		if(Uart1Tx.counter >= Uart1Tx.size)  {										//whole message is transmitted
			USART_ptr(USART_1)->CR1 &= ~USART_CR1_TCIE;								//disable transfer complete interrupt
			HardwareEvents &= ~HWE_UART1_TRANSMITTING;								//software may send next message
		} else {
			USART_ptr(USART_1)->TDR = Uart1Tx.data[Uart1Tx.counter++];				//send next byte which also clears transfer complete flag
		}
	} else {																		//unknown interrupt
		USART_ptr(USART_1)->CR1 &= ~(USART_CR1_RE | USART_CR1_TE);					//disable transmitter and receiver
		ErrorHandler("UART1 IRQHandler error");
	}
}


void USART2_IRQHandler(void)
{
	if((USART_ptr(USART_2)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(USART_2)->ISR & USART_ISR_RXNE)) { 			//new byte received
		if(Uart2RxCounter < sizeof(Uart2RxBuffer))
			Uart2RxBuffer[Uart2RxCounter++] = USART_ptr(USART_2)->RDR;
		else
			USART_ptr(USART_2)->RQR |= USART_RQR_RXFRQ;								//drop the byte, but clear USART_ISR_RXNE flag

		if(HardwareEvents & HWE_UART2_RX_EVENT)
			boardRedLedToggle();

		HardwareEvents |= HWE_UART2_RX_EVENT;

	} else if((USART_ptr(USART_2)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(USART_2)->ISR & USART_ISR_ORE)) {	//overrun error occurred
		USART_ptr(USART_2)->ICR |= USART_ICR_ORECF;									//clear overrun flag
		//TODO: mark the message as invalid
	} else if((USART_ptr(USART_2)->CR1 | USART_CR1_TCIE) && (USART_ptr(USART_2)->ISR & USART_ISR_TC)) {		//byte transmission finished
		if(Uart2Tx.counter >= Uart2Tx.size) {										//whole message is transmitted
			USART_ptr(USART_2)->CR1 &= ~USART_CR1_TCIE;								//disable transfer complete interrupt
			HardwareEvents &= ~HWE_UART2_TRANSMITTING;								//software may send next message
		} else
			USART_ptr(USART_2)->TDR = Uart2Tx.data[Uart2Tx.counter++];				//send next byte which also clears transfer complete flag
	} else {																								//unknown interrupt
		USART_ptr(USART_2)->CR1 &= ~(USART_CR1_RE | USART_CR1_TE);					//disable transmitter and receiver
		ErrorHandler("USART_2 IRQHandler error");
	}
}


void LPUART1_IRQHandler(void)
{
	if((USART_ptr(LPUART)->CR1 | USART_CR1_RXNEIE) &&  (USART_ptr(LPUART)->ISR & USART_ISR_RXNE))	{		//a new byte received
		if(LpUartRxCounter < sizeof(LpUartRxBuffer))
			LpUartRxBuffer[LpUartRxCounter++] = USART_ptr(LPUART)->RDR;
		else
			USART_ptr(LPUART)->RQR |= USART_RQR_RXFRQ;								//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_LPUART_RX_EVENT;
	} else if((USART_ptr(LPUART)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(LPUART)->ISR & USART_ISR_ORE)) {	//overrun error occurred
		USART_ptr(LPUART)->ICR |= USART_ICR_ORECF;									//clear overrun flag
		//TODO: mark the message as invalid
	} else if((USART_ptr(LPUART)->CR1 | USART_CR1_TCIE) && (USART_ptr(LPUART)->ISR & USART_ISR_TC)) {		//byte transmission finished
		if(LpUartTx.counter >= LpUartTx.size)	{										//whole message is transmitted
			USART_ptr(LPUART)->CR1 &= ~USART_CR1_TCIE;								//disable transfer complete interrupt
//			USARTx_ICR(LPUART) |= USART_ICR_TCCF;									//Clear transfer complete flag. Enable transmitter to set it for next transmission
			HardwareEvents &= ~HWE_LPUART_TRANSMITTING;								//software may send next message
		} else
			USART_ptr(LPUART)->TDR = LpUartTx.data[LpUartTx.counter++];				//send next byte which also clears transfer complete flag
	} else {																		//unknown interrupt
		//USARTx_ICR(LPUART) |= ~0U;
		USART_ptr(LPUART)->CR1 &= ~(USART_CR1_RE | USART_CR1_TE);					//disable transmitter and receiver
		ErrorHandler("LPUART IRQHandler error");
	}
}


//This function handles RTC Alarm interrupt request
void RTC_IRQHandler(void)
{
	if( (RTC->ISR & RTC_ISR_ALRAF) && (RTC->CR & RTC_CR_ALRAIE) )					//if AlarmA interrupt flag is set and AlarmA interrupt is enabled
	{
		HardwareEvents |= HWE_ALARM_A;

		RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);		//Clear the AlarmA interrupt pending bit
	}

	if( LL_RTC_IsActiveFlag_ALRB(RTC) && (RTC->CR & RTC_CR_ALRBIE) )				//if AlarmB interrupt flag is set and AlarmB interrupt is enabled
	{
		RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);		//Clear the AlarmB interrupt pending bit
		LL_RTC_ALMB_Disable(RTC);

		HardwareEvents |= timerSchedule[0].event;
		timerSchedule[0].event = HWE_NO_EVENT;
/*
		if(timerSchedule[1].event) {												//there are more scheduled events, set AlarmB for the next one
			memmove(&timerSchedule[0], &timerSchedule[1], (TIMER_SCHEDULE_SIZE - 1) * sizeof(TimerQueue));
			timerSchedule[TIMER_SCHEDULE_SIZE - 1].event = HWE_NO_EVENT;			//make sure the last entry is free to use now

			while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
			LL_RTC_ALMB_SetSubSecond(RTC, 1024 - timerSchedule[0].subseconds);
			LL_RTC_ALMB_SetSecond(RTC, __LL_RTC_CONVERT_BIN2BCD(timerSchedule[0].seconds));
			LL_RTC_ALMB_SetMinute(RTC, __LL_RTC_CONVERT_BIN2BCD(timerSchedule[0].minutes));
			LL_RTC_ALMB_Enable(RTC);
		}
*/	}

	EXTI->PR = EXTI_PR_PIF17;														//Clear the EXTI's line Flag for RTC Alarms
}


void LPTIM1_IRQHandler(void)
{
//	boardRedLedToggle();
	LL_LPTIM_ClearFLAG_CMPM(LPTIM1);	//Clear Compare match flag

	if(TimerCounter && !(--TimerCounter)){
		uint32_t i, next = TIMER_SCHEDULE_SIZE;

		//find the expired event(s), note* there can be no such events, that is when current event was rescheduled for later, then just schedule the next closest event
		//at the same time find the next event to schedule
		for(i = 0; i < TIMER_SCHEDULE_SIZE; i++)
			if(timerSchedule[i].event != HWE_NO_EVENT)
				if (timerSchedule[i].timeout) {										//one of the next events to trigger
					if ((next == TIMER_SCHEDULE_SIZE) || (timerSchedule[i].timeout < timerSchedule[next].timeout))
						next = i;
				} else {															//this is an expired event
					HardwareEvents |= timerSchedule[i].event;
					timerSchedule[i].event = HWE_NO_EVENT;
				}

		//schedule next event
		if (next != TIMER_SCHEDULE_SIZE) {
			TimerCounter = timerSchedule[next].timeout;								//set next event timeout

			for(i = 0; i < TIMER_SCHEDULE_SIZE; i++)								//update other events timeout
				timerSchedule[i].timeout -= TimerCounter;							//this would also zero just scheduled event time
		}
   }

	EXTI->PR = EXTI_IMR_IM29;														//Clear the EXTI's line Flag for LP timer
}



