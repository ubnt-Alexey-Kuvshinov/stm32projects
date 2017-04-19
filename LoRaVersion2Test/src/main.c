#include "stm32L05x_board.h"
#include "string.h"


#define CLEAR_EVENT(event) __disable_irq(); HardwareEvents &= ~event; __enable_irq()



struct SystemState DeviceState;
uint16_t HardwareEvents;
uint16_t SoftwareEvents;

uint8_t UartRxBuffer[32];
uint8_t UartRxCounter;

LpUart1Tx LpUartTx;

UartTx Uart2Tx;

int32_t tmp;
uint8_t uint8tmp;



int main(void)
{
	init_Board_Peripherals();										//Initialize all used peripherals
	boardLedToggle();
	startLpTimer(1000);

	sendLpUartMessage((uint8_t *)"hello world\r\n", 13);

	while(1)
	{
		if (HardwareEvents & HWE_LPTIMER) {
			HardwareEvents &= ~HWE_LPTIMER;
			sendLpUartMessage((uint8_t *)"hello world\r\n", 13);
			boardLedToggle();
			startLpTimer(1000);
		}
	}
}

