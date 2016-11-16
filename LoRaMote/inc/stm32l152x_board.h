#ifndef __STM32L152X_BOARD_H
#define __STM32L152X_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l1xx.h"
#include "stm32l1xx_ll_cortex.h"
//#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_rtc.h"
#include "stm32l1xx_ll_exti.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_i2c.h"
#include "stdlib.h"
#include "radio.h"
#include "gps.h"
#include "altimeter.h"
#include <stdio.h>
#include <string.h>


#define GPIO_ptr(base)			((volatile GPIO_TypeDef *)(base))			//GPIO port A, B or C registers structure
#define USART_ptr(base)			((volatile USART_TypeDef *)(base))			//USARTs and LPUART (which we do not have) registers structure

#define USARTx_RX_INTERRUPT_DISABLE(base)	USART_ptr(base)->CR1 &= ~USART_CR1_RXNEIE
#define USARTx_RX_INTERRUPT_ENABLE(base)	USART_ptr(base)->CR1 |= USART_CR1_RXNEIE

#define CLEAR_EVENT(event) __disable_irq(); HardwareEvents &= ~(event); __enable_irq()


 typedef enum
 {
 	USART_1					= USART1_BASE,
 	USART_2					= USART2_BASE,
 } USARTx;

 typedef enum
 {
 	PORT_A					= GPIOA_BASE,
 	PORT_B					= GPIOB_BASE,
 	PORT_C					= GPIOC_BASE,
	PORT_D					= GPIOD_BASE,
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
#define AUX_GREEN_LED_PORT			PORT_C
#define AUX_GREEN_LED_BIT			GPIO_3

#define AUX_YELLOW_LED_PORT			PORT_B
#define AUX_YELLOW_LED_BIT			GPIO_10

#define AUX_RED_LED_PORT			PORT_B
#define AUX_RED_LED_BIT				GPIO_1

//Buttons
#define BUTTON_PORT					PORT_A
#define BUTTON_BIT					GPIO_0

//#define AUX_BUTTON_PORT		PORT_A
//#define AUX_BUTTON_BIT		GPIO_14

//UART
#define UART1_TX_PORT				PORT_B
#define UART1_TX_BIT				GPIO_6

#define UART1_RX_PORT				PORT_B
#define UART1_RX_BIT				GPIO_7

#define UART2_TX_PORT				PORT_A
#define UART2_TX_BIT				GPIO_2

#define UART2_RX_PORT				PORT_A
#define UART2_RX_BIT				GPIO_3

//Radio
#define RADIO_DIO0_PORT				PORT_C
#define RADIO_DIO0_BIT				GPIO_6

#define RADIO_DIO1_PORT				PORT_C
#define RADIO_DIO1_BIT				GPIO_10

#define RADIO_DIO2_PORT				PORT_C
#define RADIO_DIO2_BIT				GPIO_11

#define RADIO_DIO3_PORT				PORT_C
#define RADIO_DIO3_BIT				GPIO_8

#define RADIO_DIO4_PORT				PORT_C
#define RADIO_DIO4_BIT				GPIO_9

#define RADIO_DIO5_PORT				PORT_C
#define RADIO_DIO5_BIT				GPIO_12

#define RADIO_RESET_PORT			PORT_C
#define RADIO_RESET_BIT				GPIO_2

#define RADIO_NSS_PORT				PORT_B
#define RADIO_NSS_BIT				GPIO_12

#define RADIO_MOSI_PORT				PORT_B
#define RADIO_MOSI_BIT				GPIO_15

#define RADIO_MISO_PORT				PORT_B
#define RADIO_MISO_BIT				GPIO_14

#define RADIO_SCK_PORT				PORT_B
#define RADIO_SCK_BIT				GPIO_13

#define RF_SWITCH_CONTROL_1_PORT	PORT_C
#define RF_SWITCH_CONTROL_1_BIT		GPIO_4

#define RF_SWITCH_CONTROL_2_PORT	PORT_C
#define RF_SWITCH_CONTROL_2_BIT		GPIO_13

#define RF_PA_HIGH_PWR_PORT			PORT_D
#define RF_PA_HIGH_PWR_BIT			GPIO_2

//GPS
#define GPS_ENABLE_PORT				PORT_B
#define GPS_ENABLE_BIT				GPIO_11

//I2C
#define I2C1_SCL_PORT				PORT_B
#define I2C1_SCL_BIT				GPIO_8

#define I2C1_SDA_PORT				PORT_B
#define I2C1_SDA_BIT				GPIO_9


typedef enum
{
	HWE_NO_EVENT			= 0b00000000000000000000,					//initial state - no events
	HWE_UART1_RX_EVENT		= 0b00000000000000000001,					//a new character was received by UART1 (GPS)
	HWE_UART2_RX_EVENT		= 0b00000000000000000010,					//a new character was received by UART2
	HWE_UART1_COMMAND		= 0b00000000000000000100,					//characters received by UART1 made up a full command
	HWE_UART2_COMMAND		= 0b00000000000000001000,					//characters received by UART2 made up a full command
	HWE_BUTTON_PRESS		= 0b00000000000000010000,					//button was pressed
	HWE_BUTTON_RELEASE		= 0b00000000000000100000,					//button was released
	HWE_ALARM_A				= 0b00000000000001000000,					//Alarm A signaled by RTC
	HWE_LPTIMER				= 0b00000000000010000000,					//generic timer event
	HWE_INPUT_TIMEOUT		= 0b00000000000100000000,					//user input inactivity timer
	HWE_RADIO_TIMEOUT		= 0b00000000001000000000,					//user input inactivity timer
	HWE_GPS_TIMEOUT			= 0b00000000010000000000,					//waiting for GPS to get first positioning
//	HWE_GPS_DATA			= 0b00000000100000000000,					//GPS UART received a message from the chip
	HWE_DIO0				= 0b00000001000000000000,					//interrupt on DIO0 line, can be RX, TX or CAD done event
	HWE_DIO3				= 0b00000010000000000000,					//interrupt on DIO3 line, can be reception of a valid header or PayloadCrcError
//artificial events indicating activities in progress, meant to prevent MCU going to low-power mode to allow the activities to finish
	HWE_INPUT_WAITING		= 0b00000100000000000000,					//to allow user commands from UART2 as it does not work in Stop mode (not needed for STM053)
	HWE_UART1_RECEIVING		= 0b00001000000000000000,					//waiting for GPS to report coordinates, reset after first fix, UART1 may keep running and GPS keep updating after resetting this flag
	HWE_UART2_TRANSMITTING	= 0b00010000000000000000,					//message transmission is in progress, reset by ISR
	HWE_RADIO_RECEIVING		= 0b00100000000000000000,					//radio is receiving
	HWE_RADIO_TRANSMITTING	= 0b01000000000000000000,					//radio is transmitting
	HWE_KEEP_CPU_RUNNING	= 0b10000000000000000000,					//prevent deep sleep
} Hardware_Events;

typedef enum
{
	SWE_NO_EVENT			= 0b0000,								//initial state - no events
	SWE_COMMAND				= 0b0001,								//a new command received
} Software_Events;

typedef enum
{
	PWR_STANDBY,														//no power is needed, device can go to StandBy mode
	PWR_SLEEP,															//at least maintaining MCU output states is required
	PWR_FULL															//need full MCU capability
} PowerRequirement;

typedef enum
{
	STATE_IDLE				= 0,									//all the current work is done may go to deep sleep
	STATE_STARTING,													//system powered up or reset line was triggered
	STATE_ALARM,													//processing RTC wake-up alarm
	STATE_EXT_EVENT,												//processing wake-up from accelerometer
	STATE_WAITING_INPUT,											//waiting for user input
//	STATE_RADIO_TRANSMITTING,										//transmitting from the radio
//	STATE_RADIO_RECEIVING,											//radio is receiving
} System_States;

typedef enum
{
	STATE_INIT				= 0,									//entering the state for the first time
	STATE_LED_CADENCE_1,											//first LED change delay
	STATE_LED_CADENCE_2,											//second LED change delay
	STATE_BUSINESS,													//do something
} System_SubStates;

extern struct SystemState {
	union {
		struct {
			uint8_t state;
			uint8_t substate;
		};
		uint16_t StateAndSubstate;
	};
	uint16_t OldStateAndSubstate;									//previous state/substate for change detection
} DeviceState;

typedef struct Uart {
	uint8_t data[256 + 16];
	uint16_t size;
	uint16_t counter;
} UartTx;


#define timer_milliseconds(x)	(((x) * 1024) / 1000)				//this is a correction as timer interval is not 1/1000th of a second, but 1/1024th
#define timer_seconds(x)		((x) * 1024)
#define timers_minutes(x)		((x) * timer_seconds(60))

#define TIMER_SCHEDULE_SIZE	4										//max number of events that can be scheduled

typedef struct {
	union {
		struct  {
			uint16_t subseconds;									//subseconds to match for AlarmB trigger
			uint8_t seconds;										//seconds to match for AlarmB trigger
			uint8_t minutes;										//minutes to match for AlarmB trigger
		};
		int32_t time;												//time components combined for comparison
	};
	uint32_t event;													//event to trigger when it's time expires
} TimerQueue;


extern uint32_t HardwareEvents;										//combination of all current hardware events
extern uint16_t SoftwareEvents;										//e.g. new command received
extern uint16_t OldStateAndSubstate;

extern uint8_t UartRxBuffer[32];
extern uint8_t UartRxCounter;
extern UartTx Uart2Tx;

extern TimerQueue timerSchedule[TIMER_SCHEDULE_SIZE];

extern int32_t tmp;
extern uint8_t uint8tmp;
extern uint32_t uint32tmp;

void init_Board_Peripherals(void);

static inline void setPortBit(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->BSRR = (0x01 << pin);
}

static inline void resetPortBit(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->BSRR = (0x01 << (pin + 16));
}


//void setPortBit(GPIO_Port base, GPIO_Number pin);
//void resetPortBit(GPIO_Port base, GPIO_Number pin);
uint32_t readPortBit(GPIO_Port base, GPIO_Number pin);
//void drivePortBit(GPIO_Port base, GPIO_Number pin, uint8_t high_low);

void boardGreenLedOn(void);
void boardGreenLedOff(void);
void boardGreenLedToggle(void);
void boardYellowLedOn(void);
void boardYellowLedOff(void);
void boardYellowLedToggle(void);
void boardRedLedOn(void);
void boardRedLedOff(void);
void boardRedLedToggle(void);
void driveGreenLed(uint8_t high_low);
void driveRedLed(uint8_t high_low);

void driveRfSwitch(uint8_t high_low);

int32_t uart2Receive(uint8_t *data, uint32_t length);
void uart2Transmit(uint8_t *data, uint32_t length);
void startUart2Msg(uint8_t command);
void addToUart2Msg(uint8_t byte);
void uart2SendText(char *text);
void sendUart2Message(void);
void addToUart2MsgAsHex(uint8_t byte);

void spiAccessRegisters(uint8_t *data, uint8_t length);
void spiAccessRegisters2(uint8_t command, uint8_t *data, uint8_t length);

//void startLpTimer(uint16_t timeout);
void setTimer(uint32_t timeout, uint32_t event);

void EnableGps(void);
void DisableGps(void);

void enterSleep(void);
void enterStandby(void);

void ErrorHandler(char *msg);

    
#ifdef __cplusplus
}
#endif

#endif // __STM32L152X_BOARD_H


