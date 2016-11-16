
#ifndef __STM32F103RB_BOARD_H
#define __STM32F103RB_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx.h"
#include "stdlib.h"
#include "radio_common.h"

#define RTC_ptr					((volatile RTC_TypeDef *)RTC_BASE)			//RTC registers structure
#define LPT_ptr					((volatile LPTIM_TypeDef *)LPTIM1_BASE)		//Timer registers structure
#define SPI1_ptr				((volatile SPI_TypeDef *)SPI1_BASE)			//SPI registers structure
#define TIM3_ptr				((volatile TIM_TypeDef *)TIM3_BASE)			//Timer 3 registers structure
#define GPIO_ptr(base)			((volatile GPIO_TypeDef *)(base))
#define USART_ptr(base)			((volatile USART_TypeDef *)(base))

#define USARTx_RX_INTERRUPT_DISABLE(base)	USART_ptr(base)->CR1 &= ~USART_CR1_RXNEIE
#define USARTx_RX_INTERRUPT_ENABLE(base)	USART_ptr(base)->CR1 |= USART_CR1_RXNEIE

typedef enum
{
 	USART_1					= USART1_BASE,
 	USART_2					= USART2_BASE,
	USART_3					= USART3_BASE,
} USARTx;

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
#define LED_PORT			PORT_A
#define LED_BIT				GPIO_5

#define AUX_GREEN_LED_PORT	PORT_B
#define AUX_GREEN_LED_BIT	GPIO_7

#define AUX_RED_LED_PORT	PORT_A
#define AUX_RED_LED_BIT		GPIO_15

//Buttons
#define BUTTON_PORT			PORT_C
#define BUTTON_BIT			GPIO_13

#define AUX_BUTTON_PORT		PORT_A
#define AUX_BUTTON_BIT		GPIO_14

//UART
#define UART2_TX_PORT		PORT_A
#define UART2_TX_BIT		GPIO_2

#define UART2_RX_PORT		PORT_A
#define UART2_RX_BIT		GPIO_3

//Radio
#define RADIO_DIO0_PORT		PORT_A
#define RADIO_DIO0_BIT		GPIO_10

#define RADIO_DIO1_PORT		PORT_B
#define RADIO_DIO1_BIT		GPIO_3

#define RADIO_DIO2_PORT		PORT_B
#define RADIO_DIO2_BIT		GPIO_5

#define RADIO_DIO3_PORT		PORT_B
#define RADIO_DIO3_BIT		GPIO_4

#define RADIO_DIO4_PORT		PORT_A
#define RADIO_DIO4_BIT		GPIO_9

#define RADIO_DIO5_PORT		PORT_C
#define RADIO_DIO5_BIT		GPIO_7

#define RADIO_RESET_PORT	PORT_A
#define RADIO_RESET_BIT		GPIO_0

#define RADIO_RXTX_EXT_PORT	PORT_C
#define RADIO_RXTX_EXT_BIT	GPIO_1

#define RADIO_NSS_PORT		PORT_B
#define RADIO_NSS_BIT		GPIO_6

#define RADIO_MOSI_PORT		PORT_A
#define RADIO_MOSI_BIT		GPIO_7

#define RADIO_MISO_PORT		PORT_A
#define RADIO_MISO_BIT		GPIO_6

#define RADIO_SCK_PORT		PORT_A
#define RADIO_SCK_BIT		GPIO_5									//also green user LED


typedef enum
{
	HWE_NO_EVENT			= 0b000000000,							//initial state - no events
	HWE_UART2_RX_EVENT		= 0b000000001,							//a new character was received by UART
	HWE_UART2_TRANSMITTING	= 0b000000010,							//message transmission is in progress, prevents going to Stop mode, reset by ISR
	HWE_BUTTON_PRESS		= 0b000000100,							//button was pressed
	HWE_BUTTON_RELEASE		= 0b000001000,							//button was released
	HWE_ALARM_A				= 0b000010000,							//Alarm A signaled by RTC
	HWE_TIMER3				= 0b000100000,							//timer expired
	HWE_RADIO_RECEIVING		= 0b001000000,							//radio is in RX mode, so the device can not go to sleep mode
	HWE_RADIO_TRANSMITTING	= 0b010000000,							//radio is transmitting, so the device can not go to sleep mode
	HWE_DIO0				= 0b100000000,							//DIO0 line interrupt
} Hardware_Events;

typedef enum
{
	STATE_IDLE				= 0,									//all the current work is done may go to deep sleep
	STATE_STARTING,													//system powered up or reset line was triggered
	STATE_ALARM,													//processing RTC wake-up alarm
	STATE_EXT_EVENT,												//processing wake-up from accelerometer
	STATE_WAITING_INPUT,											//waiting for user input
	STATE_RADIO_TRANSMITTING,										//answering communication request
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
	uint16_t OldStateAndSubstate;									//previous state/sub state for change detection
} DeviceState;

typedef struct Uart {
	uint8_t data[256 + 16];											//FIFO size plus additional data
	uint16_t size;
	uint16_t counter;
} UartTx;


extern uint16_t HardwareEvents;										//combination of all current hardware events
extern uint16_t SoftwareEvents;										//e.g. new command received
extern uint16_t OldStateAndSubstate;

extern uint8_t UartRxBuffer[256 + 16];
extern uint16_t UartRxCounter;
extern UartTx Uart2Tx;

extern int32_t tmp;
extern uint32_t uint32tmp;

static inline void stopTimer3(void)
{
	TIM3_ptr->CR1 &= ~TIM_CR1_CEN;										//stop the timer
}

void init_Board_Peripherals(void);
void setPortBit(GPIO_Port base, GPIO_Number pin);
void resetPortBit(GPIO_Port base, GPIO_Number pin);
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
void RadioSetCommunicationParameters(uint32_t fq, RadioBW1276 bw, RadioSF sf, RadioCR cr, RadioEH eh);
void RadioTransmit(uint8_t *data, uint8_t length);
void RadioReceive(void);

void enableRtcConfig(void);
void disableRtcConfig(void);

void startUart2Msg(uint8_t command);
void addToUart2Msg(uint8_t byte);
void uart2SendText(char *text);
void sendUart2Message(void);

void spiAccessRegisters(uint8_t *data, uint8_t length);
void spiAccessRegisters2(uint8_t command, uint8_t *data, uint8_t length);

void ErrorHandler(char *msg);

void startTimer3(uint16_t timeout);

    
#ifdef __cplusplus
}
#endif

#endif // __STM32F103RB_BOARD_H


