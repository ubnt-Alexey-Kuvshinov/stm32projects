#include <stm32l152x_board.h>
//#include "stm32l1xx_it.h"



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


void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF0)					//Button
	{
		EXTI->PR |= EXTI_PR_PIF0;
		if(readPortBit(BUTTON_PORT, BUTTON_BIT))
			HardwareEvents |= HWE_BUTTON_PRESS;
		else
			HardwareEvents |= HWE_BUTTON_RELEASE;
	}
	else
		ErrorHandler("IRQHandler error");
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF6)											//PC6 (DIO0) interrupt
	{
		EXTI->PR |= EXTI_PR_PIF6;
		HardwareEvents |= HWE_DIO0;
	}
	else if(EXTI->PR & EXTI_PR_PIF8)									//PC8 (DIO3) interrupt
	{
		EXTI->PR |= EXTI_PR_PIF8;
		HardwareEvents |= HWE_DIO3;
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
	if(USART_ptr(USART_1)->SR & USART_SR_RXNE)
	{
		if(GpsRxCounter < sizeof(GpsRxBuffer))
			GpsRxBuffer[GpsRxCounter++] = USART_ptr(USART_1)->DR;
		else
			USART_ptr(USART_1)->SR &= ~USART_SR_RXNE;					//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_UART1_RX_EVENT;
	}
	else if(USART_ptr(USART_1)->SR & USART_SR_TC)
	{
		USART_ptr(USART_1)->CR1 &= ~USART_CR1_TCIE;						//disable transfer complete interrupt
	}
	else
		//USARTx_ICR(USART_1) |= ~0U;
		ErrorHandler("IRQHandler error");
}


void USART2_IRQHandler(void)
{
	if(USART_ptr(USART_2)->SR & USART_SR_RXNE)
	{
		if(UartRxCounter < sizeof(UartRxBuffer))
			UartRxBuffer[UartRxCounter++] = USART_ptr(USART_2)->DR;
		else
			USART_ptr(USART_2)->SR &= ~USART_SR_RXNE;					//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_UART2_RX_EVENT;
	}
	else if(USART_ptr(USART_2)->SR & USART_SR_TC)
	{
		if(Uart2Tx.counter >= Uart2Tx.size)								//whole message is transmitted
		{
			USART_ptr(USART_2)->CR1 &= ~USART_CR1_TCIE;					//disable transfer complete interrupt
//			USARTx_ICR(USART_2) |= USART_ICR_TCCF;						//Clear transfer complete flag. Enable transmitter to set it for next transmission
			HardwareEvents &= ~HWE_UART2_TRANSMITTING;					//software may send next message
		}
		else
			USART_ptr(USART_2)->DR = Uart2Tx.data[Uart2Tx.counter++];	//send next byte which also clears transfer complete flag
	}
	else
		//USARTx_ICR(USART_2) |= ~0U;
		ErrorHandler("IRQHandler error");
}



//This function handles RTC Alarm interrupt request
void RTC_Alarm_IRQHandler(void)
{

	if( (RTC->ISR & RTC_ISR_ALRAF) && (RTC->CR & RTC_CR_ALRAIE) )		//if AlarmA interrupt flag is set and AlarmA interrupt is enabled
	{
		HardwareEvents |= HWE_ALARM_A;

		RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
	}

	if( LL_RTC_IsActiveFlag_ALRB(RTC) && (RTC->CR & RTC_CR_ALRBIE) )	//if AlarmB interrupt flag is set and AlarmB interrupt is enabled
	{
		RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmB interrupt pending bit
		LL_RTC_ALMB_Disable(RTC);

		HardwareEvents |= timerSchedule[0].event;
		timerSchedule[0].event = HWE_NO_EVENT;

		if(timerSchedule[1].event) {									//there are more scheduled events, set AlarmB for the next one
			memmove(&timerSchedule[0], &timerSchedule[1], (TIMER_SCHEDULE_SIZE - 1) * sizeof(TimerQueue));
			timerSchedule[TIMER_SCHEDULE_SIZE - 1].event = HWE_NO_EVENT;//make sure the last entry is free to use now

			while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
			LL_RTC_ALMB_SetSubSecond(RTC, 1024 - timerSchedule[0].subseconds);
			LL_RTC_ALMB_SetSecond(RTC, __LL_RTC_CONVERT_BIN2BCD(timerSchedule[0].seconds));
			LL_RTC_ALMB_SetMinute(RTC, __LL_RTC_CONVERT_BIN2BCD(timerSchedule[0].minutes));
			LL_RTC_ALMB_Enable(RTC);
		}

	}

	EXTI->PR = EXTI_PR_PIF17;											//Clear the EXTI's line Flag for RTC Alarms
}

