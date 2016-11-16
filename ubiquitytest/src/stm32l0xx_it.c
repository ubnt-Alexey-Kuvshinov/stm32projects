#include "stm32l05x_board.h"



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
	if(EXTI->PR & EXTI_PR_PIF13)					//Button
	{
		EXTI->PR |= EXTI_PR_PIF13;
		if(readPortBit(BUTTON_PORT, BUTTON_BIT))
			HardwareEvents |= HWE_BUTTON_PRESS;
		else
			HardwareEvents |= HWE_BUTTON_RELEASE;
//		boardGreenLedToggle();
	}
	else if(EXTI->PR & EXTI_PR_PIF10)					//DIO0 from the radio
	{
		EXTI->PR |= EXTI_PR_PIF10;
		HardwareEvents |= HWE_DIO0;
	}

}


/*void PPP_IRQHandler(void)
{
}*/



void TIM6_DAC_IRQHandler(void)
{
//  HAL_TIM_IRQHandler(&TimHandle);
}


void USART2_IRQHandler(void)
{
//	if((USARTx_ISR(USART_2) & USART_ISR_RTOF) && (USARTx_CR1(USART_2) |= USART_CR1_RTOIE))
//	if(USARTx_ISR(USART_2) & USART_ISR_RTOF)
	{
//		USARTx_ICR(USART_2) |= USART_ICR_RTOCF;
	}
	if(USART_ptr(USART_2)->ISR & USART_ISR_RXNE)
	{
		if(UartRxCounter < sizeof(UartRxBuffer))
			UartRxBuffer[UartRxCounter++] = USART_ptr(USART_2)->RDR;
		else
			USART_ptr(USART_2)->RQR |= USART_RQR_RXFRQ;					//to clear USART_ISR_RXNE flag

		HardwareEvents |= HWE_UART2_RX_EVENT;
	}
	else if(USART_ptr(USART_2)->ISR & USART_ISR_TC)
	{
		if(Uart2Tx.counter >= Uart2Tx.size)							//whole message is transmitted
		{
			USART_ptr(USART_2)->CR1 &= ~USART_CR1_TCIE;					//disable transfer complete interrupt
//			USARTx_ICR(USART_2) |= USART_ICR_TCCF;					//Clear transfer complete flag. Enable transmitter to set it for next transmission
			HardwareEvents &= ~HWE_UART2_TRANSMITTING;				//software may send next message
		}
		else
			USART_ptr(USART_2)->TDR = Uart2Tx.data[Uart2Tx.counter++];	//send next byte which also clears transfer complete flag
	}
	else
		;//USARTx_ICR(USART_2) |= ~0U;

}



//This function handles RTC Alarm interrupt request
void RTC_IRQHandler(void)
{

	if( (RTC->ISR & RTC_ISR_ALRAF) && (RTC->CR & RTC_CR_ALRAIE) )	//if AlarmA interrupt flag is set and AlarmA interrupt is enabled
	{
		HardwareEvents |= HWE_ALARM_A;

		RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
	}

	if( (RTC->ISR & RTC_ISR_ALRBF) && (RTC->CR & RTC_CR_ALRBIE) )	//if AlarmB interrupt flag is set and AlarmB interrupt is enabled
	{
	  // do something
	  ;

	  RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmB interrupt pending bit
	}

	EXTI->PR = EXTI_PR_PIF17;												//Clear the EXTI's line Flag for RTC Alarm
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

	  EXTI->PR = EXTI_IMR_IM29;												//Clear the EXTI's line Flag for LP timer
}
