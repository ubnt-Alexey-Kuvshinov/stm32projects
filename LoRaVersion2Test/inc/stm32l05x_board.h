
#ifndef __STM32L05X_BOARD_H
#define __STM32L05X_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif


//#include "stm32l051xx.h"
#include "stm32l052xx.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_lptim.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stdlib.h"



#define GPIO_ptr(base)			((volatile GPIO_TypeDef *)(base))			//GPIO port A, B or C registers structure
#define USART_ptr(base)			((volatile USART_TypeDef *)(base))			//USARTs and LPUART (which we do not have) registers structure

#define USARTx_RX_INTERRUPT_DISABLE(base)	USART_ptr(base)->CR1 &= ~USART_CR1_RXNEIE
#define USARTx_RX_INTERRUPT_ENABLE(base)	USART_ptr(base)->CR1 |= USART_CR1_RXNEIE


 typedef enum
 {
 	USART_1					= USART1_BASE,
 	USART_2					= USART2_BASE,
 	LPUART					= LPUART1_BASE,
 } USARTx;

 typedef enum
 {
 	US_USART_14400			= 1111,
 	US_USART_115200			= 0x8B,
 	US_LPUART_115200		= 35555
 } UartSpeed;

 typedef struct {
 	uint8_t data[32];
 	uint16_t size;
 	uint16_t counter;
 } LpUart1Tx;

 typedef enum
 {
 	PORT_A					= GPIOA_BASE,
 	PORT_B					= GPIOB_BASE,
 	PORT_C					= GPIOC_BASE,
 } GPIO_Port;

 typedef enum
 {
 	GPIO_0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
 	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
 	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
 	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
 } GPIO_Number;


//LEDs
#define LED_PORT			PORT_C
#define LED_BIT				GPIO_11

//UART
#define UART1_TX_PORT				PORT_A								//this is marked as UART3 on schematic
#define UART1_TX_BIT				GPIO_9

#define UART1_RX_PORT				PORT_A
#define UART1_RX_BIT				GPIO_10

#define LPUART_TX_PORT				PORT_B								//this is marked as UART1 on schematic
#define LPUART_TX_BIT				GPIO_10

#define LPUART_RX_PORT				PORT_B
#define LPUART_RX_BIT				GPIO_11


typedef enum
{
	HWE_NO_EVENT			= 0b0000000,							//initial state - no events
	HWE_LPUART_RX_EVENT		= 0b0000001,							//a new character was received by LPUART (GPS)
	HWE_LPUART_TRANSMITTING	= 0b0000010,							//transmission to GPS is in progress, reset by ISR
	HWE_LPTIMER				= 0b0000100,							//timer expired
} Hardware_Events;

typedef enum
{
	STATE_IDLE				= 0,									//all the current work is done may go to deep sleep
	STATE_STARTING,													//system powered up or reset line was triggered
	STATE_ALARM,													//processing RTC wake-up alarm
	STATE_EXT_EVENT,												//processing wake-up from accelerometer
	STATE_WAITING_INPUT,											//waiting for user input
} System_States;

typedef enum
{
	STATE_INIT				= 0,									//entering the state for the first time
	STATE_LED_CADENCE_1,											//first LED change delay
	STATE_LED_CADENCE_2,											//second LED change delay
	STATE_BUSINESS,													//do something
} System_SubStates;

typedef enum
{
	SWE_NO_EVENT			= 0b0000,								//initial state - no events
	SWE_COMMAND				= 0b0001,								//a new command received
} Software_Events;


extern struct SystemState {
	union {
		struct {
			uint8_t state;
			uint8_t substate;
		};
		uint16_t StateAndSubstate;
	};
	uint16_t OldStateAndSubstate;								//previous state/substate for change detection
} DeviceState;

typedef struct Uart {
	uint8_t data[32];
	uint8_t size;
	uint8_t counter;
} UartTx;

extern uint16_t HardwareEvents;										//combination of all current hardware events
extern uint16_t SoftwareEvents;										//e.g. new command received
extern uint16_t OldStateAndSubstate;

extern uint8_t UartRxBuffer[32];
extern uint8_t UartRxCounter;
extern UartTx Uart2Tx;
extern LpUart1Tx LpUartTx;

extern int32_t tmp;

void init_Board_Peripherals(void);

//void setPortBit(GPIO_Port base, GPIO_Number pin);
//void resetPortBit(GPIO_Port base, GPIO_Number pin);
uint32_t readPortBit(GPIO_Port base, GPIO_Number pin);
//void drivePortBit(GPIO_Port base, GPIO_Number pin, uint8_t high_low);

void boardGreenLedOn(void);
void boardGreenLedOff(void);
void boardGreenLedToggle(void);
void boardRedLedOn(void);
void boardRedLedOff(void);
void boardRedLedToggle(void);
void driveGreenLed(uint8_t high_low);
void driveRedLed(uint8_t high_low);

void driveRfSwitch(uint8_t high_low);

void uart1Transmit(uint8_t *data, uint32_t length);
void uart2Transmit(uint8_t *data, uint32_t length);

void spiAccessRegisters(uint8_t *data, uint8_t length);

void startLpTimer(uint16_t timeout);

void enterSleep(void);
void enterStandby(void);

void ErrorHandler(char *msg);

    
#ifdef __cplusplus
}
#endif

#endif // __STM32L05X_BOARD_H


