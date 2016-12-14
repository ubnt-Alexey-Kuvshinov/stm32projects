#include "stm32L051x_board.h"

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{

}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
	  //void boardRedLedOn(void);
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void EXTI4_15_IRQHandler(void)
{
/*	if(EXTI->PR & EXTI_PR_PIF13)					//Button
	{
		EXTI->PR |= EXTI_PR_PIF13;
		if(readPortBit(BUTTON_PORT, BUTTON_BIT))
			HardwareEvents |= HWE_BUTTON_PRESS;
		else
			HardwareEvents |= HWE_BUTTON_RELEASE;
//		boardGreenLedToggle();
	}
	else */if(EXTI->PR & EXTI_PR_PIF12)					//DIO0 from the radio
	{
		EXTI->PR |= EXTI_PR_PIF12;
		HardwareEvents |= HWE_DIO0;
	}
	else
		ErrorHandler("IRQHandler error");
}


void TIM6_DAC_IRQHandler(void)
{
//  HAL_TIM_IRQHandler(&TimHandle);
}


void USART1_IRQHandler(void)
{
	if((USART_ptr(USART_1)->CR1 | USART_CR1_RXNEIE) &&  (USART_ptr(USART_1)->ISR & USART_ISR_RXNE))	{		//a new byte received
		if(Uart1RxCounter < sizeof(Uart1RxBuffer))
			Uart1RxBuffer[Uart1RxCounter++] = USART_ptr(USART_1)->RDR;
		else
			USART_ptr(USART_1)->RQR |= USART_RQR_RXFRQ;								//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_UART1_RX_EVENT;
	} else if((USART_ptr(USART_1)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(USART_1)->ISR & USART_ISR_ORE)) {	//overrun error occurred
		USART_ptr(USART_1)->ICR |= USART_ICR_ORECF;									//clear overrun flag
		//TODO: mark the message as invalid
	} else if((USART_ptr(USART_1)->CR1 | USART_CR1_TCIE) && (USART_ptr(USART_1)->ISR & USART_ISR_TC)) {		//byte transmission finished
		if(Uart1Tx.counter >= Uart1Tx.size)	{										//whole message is transmitted
			USART_ptr(USART_1)->CR1 &= ~USART_CR1_TCIE;								//disable transfer complete interrupt
//			USARTx_ICR(USART_1) |= USART_ICR_TCCF;									//Clear transfer complete flag. Enable transmitter to set it for next transmission
			HardwareEvents &= ~HWE_UART1_TRANSMITTING;								//software may send next message
		} else
			USART_ptr(USART_1)->TDR = Uart1Tx.data[Uart1Tx.counter++];				//send next byte which also clears transfer complete flag
	} else {																		//unknown interrupt
		//USARTx_ICR(USART_1) |= ~0U;
		USART_ptr(USART_1)->CR1 &= ~(USART_CR1_RE | USART_CR1_TE);					//disable transmitter and receiver
		ErrorHandler("IRQHandler error");
	}
}


void USART2_IRQHandler(void)
{
	if((USART_ptr(USART_2)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(USART_2)->ISR & USART_ISR_RXNE)) { 		//new byte received
		if(Uart2RxCounter < sizeof(Uart2RxBuffer))
			Uart2RxBuffer[Uart2RxCounter++] = USART_ptr(USART_2)->RDR;
		else
			USART_ptr(USART_2)->RQR |= USART_RQR_RXFRQ;								//drop the byte, but clear USART_ISR_RXNE flag

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
	if((USART_ptr(LPUART)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(LPUART)->ISR & USART_ISR_RXNE)) {			//new byte received
		if(GpsRxCounter < sizeof(GpsRxBuffer))
			GpsRxBuffer[GpsRxCounter++] = USART_ptr(LPUART)->RDR;
		else
			USART_ptr(LPUART)->RQR |= USART_RQR_RXFRQ;								//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_LPUART_RX_EVENT;
	} else if((USART_ptr(LPUART)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(LPUART)->ISR & USART_ISR_ORE)) {	//overrun error occurred																//then it was triggered because of overrun error
		USART_ptr(LPUART)->ICR |= USART_ICR_ORECF;								//clear overrun flag
		//TODO: mark the message as invalid
	} else if((USART_ptr(LPUART)->CR1 | USART_CR1_TCIE) && (USART_ptr(LPUART)->ISR & USART_ISR_TC)) {		//byte transmission finished
		if(LpUartTx.counter >= LpUartTx.size)  {									//whole message is transmitted
			USART_ptr(LPUART)->CR1 &= ~USART_CR1_TCIE;								//disable transfer complete interrupt
			HardwareEvents &= ~HWE_LPUART_TRANSMITTING;								//software may send next message
		} else {
			USART_ptr(LPUART)->TDR = LpUartTx.data[LpUartTx.counter++];				//send next byte which also clears transfer complete flag
		}
	} else {																		//unknown interrupt
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

		if(timerSchedule[1].event) {												//there are more scheduled events, set AlarmB for the next one
			memmove(&timerSchedule[0], &timerSchedule[1], (TIMER_SCHEDULE_SIZE - 1) * sizeof(TimerQueue));
			timerSchedule[TIMER_SCHEDULE_SIZE - 1].event = HWE_NO_EVENT;			//make sure the last entry is free to use now

			while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
			LL_RTC_ALMB_SetSubSecond(RTC, 1024 - timerSchedule[0].subseconds);
			LL_RTC_ALMB_SetSecond(RTC, __LL_RTC_CONVERT_BIN2BCD(timerSchedule[0].seconds));
			LL_RTC_ALMB_SetMinute(RTC, __LL_RTC_CONVERT_BIN2BCD(timerSchedule[0].minutes));
			LL_RTC_ALMB_Enable(RTC);
		}
	}

	EXTI->PR = EXTI_PR_PIF17;														//Clear the EXTI's line Flag for RTC Alarms
}


void LPTIM1_IRQHandler(void)
{
//	boardRedLedToggle();

	if(LL_LPTIM_IsActiveFlag_CMPM(LPTIM1))		//Compare match interrupt
	  {
//	    if(__HAL_LPTIM_GET_IT_SOURCE(hlptim, LPTIM_IT_CMPM) !=RESET)
	    {
	    	LL_LPTIM_ClearFLAG_CMPM(LPTIM1);	//Clear Compare match flag
	    	HardwareEvents |= HWE_LPTIMER;
	    }
	  }

	  EXTI->PR = EXTI_IMR_IM29;														//Clear the EXTI's line Flag for LP timer
}