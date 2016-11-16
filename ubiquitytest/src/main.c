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



int main(void)
{
	init_Board_Peripherals();										//Initialize all used peripherals

	startLpTimer(1000);

	while(1)
	{

		//lpUartTransmit((uint8_t *)"hello world\r\n", 13);
		if (HardwareEvents & HWE_LPTIMER) {
			HardwareEvents &= ~HWE_LPTIMER;
			uart2Transmit((uint8_t *)"hello world\r\n", 13);
			boardGreenLedToggle();
			startLpTimer(1000);
		}


		//enterSleep();
	}

}

