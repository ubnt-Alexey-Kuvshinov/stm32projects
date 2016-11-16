#include <stm32F103RB_board.h>



/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                        */
/******************************************************************************/

void NMI_Handler(void)
{
	  while (1);
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
	  //void boardRedLedOn(void);
  }
}

void SVC_Handler(void)
{
	  while (1);
}

void DebugMon_Handler(void)
{
	  while (1);
}


void PendSV_Handler(void)
{
	  while (1);
}

void SysTick_Handler(void)
{
	  while (1);
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF13)						//Button
	{
		EXTI->PR |= EXTI_PR_PIF13;
		if(readPortBit(BUTTON_PORT, BUTTON_BIT))
			HardwareEvents |= HWE_BUTTON_PRESS;
		else
			HardwareEvents |= HWE_BUTTON_RELEASE;
	}
	else if(EXTI->PR & EXTI_PR_PIF10)					//DIO0 from the radio
	{
		EXTI->PR |= EXTI_PR_PIF10;
		HardwareEvents |= HWE_DIO0;
	}
	else
		ErrorHandler("IRQHandler error");
}


void TIM6_DAC_IRQHandler(void)
{
	  while (1);
//  HAL_TIM_IRQHandler(&TimHandle);
}


void USART2_IRQHandler(void)
{
	if(USART_ptr(USART_2)->SR & USART_SR_RXNE)
	{

//		if(USART_ptr(USART_2)->SR & (~(USART_SR_RXNE | USART_SR_TC | USART_SR_TXE)))
//			while(1);

		if(UartRxCounter < sizeof(UartRxBuffer))
		{
			UartRxBuffer[UartRxCounter++] = USART_ptr(USART_2)->DR;
		}
		else
			USART_ptr(USART_2)->SR &= ~USART_SR_RXNE;					//to clear USART_SR_RXNE flag

		HardwareEvents |= HWE_UART2_RX_EVENT;
	}
	else if(USART_ptr(USART_2)->SR & USART_SR_TC)
	{
		if(Uart2Tx.counter >= Uart2Tx.size)								//whole message is transmitted
		{
			USART_ptr(USART_2)->CR1 &= ~USART_CR1_TCIE;					//disable transfer complete interrupt
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
//void RTC_IRQHandler(void)
void RTC_Alarm_IRQHandler(void)
{

/*
	if( (RTC_ptr->ISR & RTC_ISR_ALRAF) && (RTC_ptr->CR & RTC_CR_ALRAIE) )	//if AlarmA interrupt flag is set and AlarmA interrupt is enabled
	{
		HardwareEvents |= HWE_ALARM_A;

		RTC_ptr->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC_ptr->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
	}

	if( (RTC_ptr->ISR & RTC_ISR_ALRBF) && (RTC_ptr->CR & RTC_CR_ALRBIE) )	//if AlarmB interrupt flag is set and AlarmB interrupt is enabled
	{
	  // do something
	  ;

	  RTC_ptr->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC_ptr->ISR & RTC_ISR_INIT);	//Clear the AlarmB interrupt pending bit
	}

	EXTI->PR = EXTI_PR_PIF17;												//Clear the EXTI's line Flag for RTC Alarm
	*/

	boardRedLedToggle();
	RTC_ptr->CRL = ~RTC_CRL_ALRF;
	EXTI->PR = ((uint32_t)EXTI_IMR_MR17);
}


void TIM3_IRQHandler(void)
{
	if(TIM3_ptr->SR & TIM_SR_UIF)											//TIM Update event
	{
		TIM3_ptr->CR1 &= ~TIM_CR1_CEN;										//* stop the timer by configuring one shot op mode
		TIM3_ptr->SR = ~TIM_SR_UIF;											//clear interrupt flag
		HardwareEvents |= HWE_TIMER3;
	}
}
