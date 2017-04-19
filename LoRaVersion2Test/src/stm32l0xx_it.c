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



/*void PPP_IRQHandler(void)
{
}*/


void LPTIM1_IRQHandler(void)
{
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


void RNG_LPUART1_IRQHandler(void)
{
	if((USART_ptr(LPUART)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(LPUART)->ISR & USART_ISR_RXNE)) {			//new byte received
			USART_ptr(LPUART)->RQR |= USART_RQR_RXFRQ;								//to clear USART_ISR_RXNE flag
	} else if((USART_ptr(LPUART)->CR1 | USART_CR1_RXNEIE) && (USART_ptr(LPUART)->ISR & USART_ISR_ORE)) {	//overrun error occurred																//then it was triggered because of overrun error
		USART_ptr(LPUART)->ICR |= USART_ICR_ORECF;									//clear overrun flag
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
	}
}
