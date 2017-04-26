#ifndef __STM32L051X_BOARD_H
#define __STM32L051X_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx.h"
#include "stm32l0xx_ll_cortex.h"
//#include "stm32l1xx_l0_gpio.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_lptim.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_i2c.h"

#include "stm32l0xx_hal_flash.h"


#include "stdlib.h"
#include "radio.h"
#include "gps.h"
#include "camera.h"
#include "accelerometer.h"
#include <stdio.h>
#include <string.h>


#define GPIO_ptr(base)			((volatile GPIO_TypeDef *)(base))		//GPIO port A, B or C registers structure
#define USART_ptr(base)			((volatile USART_TypeDef *)(base))		//USARTs and LPUART registers structure

#define USARTx_RX_INTERRUPT_DISABLE(base)	USART_ptr(base)->CR1 &= ~USART_CR1_RXNEIE
#define USARTx_RX_INTERRUPT_ENABLE(base)	USART_ptr(base)->CR1 |= USART_CR1_RXNEIE

#define CLEAR_EVENT(event) __disable_irq(); HardwareEvents &= ~(event); __enable_irq()

#define I2C_REQUEST_READ	0x01


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
#define WHITE_LED_PORT				PORT_C
#define WHITE_LED_BIT				GPIO_11

#define AUX_GREEN_LED_PORT			PORT_C
#define AUX_GREEN_LED_BIT			GPIO_1

#define AUX_RED_LED_PORT			PORT_C
#define AUX_RED_LED_BIT				GPIO_2

//UART
#define UART1_TX_PORT				PORT_A								//this is marked as UART3 on schematic
#define UART1_TX_BIT				GPIO_9

#define UART1_RX_PORT				PORT_A
#define UART1_RX_BIT				GPIO_10

#define UART2_TX_PORT				PORT_A
#define UART2_TX_BIT				GPIO_2

#define UART2_RX_PORT				PORT_A
#define UART2_RX_BIT				GPIO_3

#define LPUART_TX_PORT				PORT_B								//this is marked as UART1 on schematic
#define LPUART_TX_BIT				GPIO_10

#define LPUART_RX_PORT				PORT_B
#define LPUART_RX_BIT				GPIO_11

//Radio
#define RADIO_DIO0_PORT				PORT_B
#define RADIO_DIO0_BIT				GPIO_2

#define RADIO_DIO1_PORT				PORT_B
#define RADIO_DIO1_BIT				GPIO_3

#define RADIO_DIO2_PORT				PORT_B
#define RADIO_DIO2_BIT				GPIO_4

#define RADIO_DIO3_PORT				PORT_B
#define RADIO_DIO3_BIT				GPIO_5

#define RADIO_DIO4_PORT				PORT_B
#define RADIO_DIO4_BIT				GPIO_6

#define RADIO_DIO5_PORT				PORT_B
#define RADIO_DIO5_BIT				GPIO_7

#define RADIO_RESET_PORT			PORT_A
#define RADIO_RESET_BIT				GPIO_8

#define RADIO_NSS_PORT				PORT_A
#define RADIO_NSS_BIT				GPIO_4

#define RADIO_MOSI_PORT				PORT_A
#define RADIO_MOSI_BIT				GPIO_7

#define RADIO_MISO_PORT				PORT_A
#define RADIO_MISO_BIT				GPIO_6

#define RADIO_SCK_PORT				PORT_A
#define RADIO_SCK_BIT				GPIO_5

#define RF_PA_HIGH_PWR_PORT			PORT_B								//RF output source switch, Low - RFO_HF, High - PA_BOOST
#define RF_PA_HIGH_PWR_BIT			GPIO_0

#define RF_CPS_PORT					PORT_C								//gain control in receive mode: Low - receive low gain, High - receive high gain
#define RF_CPS_BIT					GPIO_10

#define RF_CSD_PORT					PORT_B								//Shutdown control, should be High for RX or TX
#define RF_CSD_BIT					GPIO_1

//accelerometer
#define GSINT1_PORT					PORT_A								//accelerometer interrupt 1
#define GSINT1_BIT					GPIO_0

#define GSINT2_PORT					PORT_A								//accelerometer interrupt 2
#define GSINT2_BIT					GPIO_1

#define I2C1_SCL_PORT				PORT_B								//I2C interface
#define I2C1_SCL_BIT				GPIO_8

#define I2C1_SDA_PORT				PORT_B
#define I2C1_SDA_BIT				GPIO_9



typedef enum
{
	HWE_NO_EVENT			= 0b0000000000000000000000,					//initial state - no events
	HWE_LPUART_RX_EVENT		= 0b0000000000000000000001,					//a new character was received by LPUART (user input)
	HWE_UART2_RX_EVENT		= 0b0000000000000000000010,					//a new character was received by UART2 (camera)
	HWE_UART1_RX_EVENT		= 0b0000000000000000000100,					//a new character was received by UART1 (GPS)
	HWE_LPUART_COMMAND		= 0b0000000000000000001000,					//characters received by LPUART made up a full command
	HWE_UART1_COMMAND		= 0b0000000000000000010000,					//characters received by UART1 made up a full command
	HWE_BUTTON_PRESS		= 0b0000000000000000100000,					//button was pressed
	HWE_BUTTON_RELEASE		= 0b0000000000000001000000,					//button was released
	HWE_ALARM_A				= 0b0000000000000010000000,					//Alarm A signaled by RTC
	HWE_DIO0				= 0b0000000000000100000000,					//interrupt on DIO0 line, can be RX, TX or CAD done event
	HWE_DIO3				= 0b0000000000001000000000,					//interrupt on DIO3 line, can be reception of a valid header or PayloadCrcError
	HWE_GENERIC_TIMEOUT		= 0b0000000000010000000000,					//generic timer event
	HWE_INPUT_TIMEOUT		= 0b0000000000100000000000,					//user input inactivity timer
	HWE_RADIO_TIMEOUT		= 0b0000000001000000000000,					//Radio task timer
	HWE_GPS_TIMEOUT			= 0b0000000010000000000000,					//GPS task timer
	HWE_CAMERA_TIMEOUT		= 0b0000000100000000000000,					//Camera task timer
	HWE_ACCEL_TIMEOUT		= 0b0000001000000000000000,					//Accelerometer task timer
//artificial events indicating activities in progress, meant to prevent MCU going to low-power mode to allow the activities to finish
	HWE_LPUART_TRANSMITTING	= 0b0000010000000000000000,					//user message transmission is in progress, reset by ISR
	HWE_UART2_TRANSMITTING	= 0b0000100000000000000000,					//transmission to camera is in progress, reset by ISR
	HWE_UART1_TRANSMITTING	= 0b0001000000000000000000,					//transmission to GPS is in progress, reset by ISR
	HWE_RADIO_RECEIVING		= 0b0010000000000000000000,					//radio is receiving
	HWE_RADIO_TRANSMITTING	= 0b0100000000000000000000,					//radio is transmitting
	HWE_KEEP_CPU_RUNNING	= 0b1000000000000000000000,					//prevent sleep
} Hardware_Events;

typedef enum
{
	SWE_NO_EVENT			= 0b0000,									//initial state - no events
	SWE_COMMAND				= 0b0001,									//a new command received
} Software_Events;

typedef enum
{
	PWR_STANDBY,														//no power is needed, device can go to StandBy mode
	PWR_SLEEP,															//at least maintaining MCU output states is required
	PWR_FULL															//need full MCU capability
} PowerRequirement;

typedef enum
{
	STATE_IDLE				= 0,										//all the current work is done may go to deep sleep
	STATE_STARTING,														//system powered up or reset line was triggered
	STATE_ALARM,														//processing RTC wake-up alarm
	STATE_EXT_EVENT,													//processing wake-up from accelerometer
	STATE_WAITING_INPUT,												//waiting for user input
	STATE_TAKING_PICTURE,												//transferring image using user output buffer, user input unavailable during this time
	STATE_TRANSMITTING_PICTURE,											//transmitting an image over the radio
	STATE_READING_ACCELEROMETER_TEMPERATURE,							//waiting for the temperature reading to become available
} System_States;

typedef enum
{
	STATE_INIT				= 0,										//entering the state for the first time
	STATE_LED_CADENCE_1,												//first LED change delay
	STATE_LED_CADENCE_2,												//second LED change delay
	STATE_BUSINESS,														//do something
} System_SubStates;

extern struct SystemState {
	union {
		struct {
			uint8_t state;
			uint8_t substate;
		};
		uint16_t StateAndSubstate;
	};
	uint16_t OldStateAndSubstate;										//previous state/substate for change detection
} DeviceState;

typedef struct {
	uint8_t data[256 + 16];
	uint16_t size;
	uint16_t counter;
} LpUart;

typedef struct {
	uint8_t data[6];
	uint16_t size;
	uint16_t counter;
} TxUart2;


#define timer_milliseconds(x)	(((x) * 1024) / 1000)					//this is a correction as timer interval is not 1/1000th of a second, but 1/1024th
#define timer_seconds(x)		((x) * 1024)
#define timers_minutes(x)		((x) * timer_seconds(60))

#define TIMER_SCHEDULE_SIZE	6											//max number of events that can be scheduled

typedef struct  {
	uint16_t timeout;													//number of LPTIM ticks remaining before triggering event
	uint16_t event;														//timer event
} TimerQueueEntry;

typedef enum {
	DDD_PICTURE_UNCONFIRMED,											//Concentrator did not confirm reception of image data
	DDD_PICTURE_RESEND,													//Concentrator requests image data retransmission
	DDD_PICTURE_NEXT,													//Concentrator confirms reception of image data
} DeviceDataDefines;

typedef struct  {
	uint8_t pictureSize;												//image size in data packets
	uint8_t picturePacketNumber;										//current image packet for transmission
	uint8_t pictureConfirmation;										//image packet transmission status: DDD_PICTURE_UNCONFIRMED / RESEND / NEXT
	uint8_t pictureRetransmissions;										//number of times same image packet was retransmitted
} DeviceDataFields;

extern DeviceDataFields DeviceData;

extern uint16_t TimerCounter;											//1/1024s down counter

extern uint32_t HardwareEvents;											//combination of all current hardware events
extern uint16_t SoftwareEvents;											//e.g. new command received
extern uint16_t OldStateAndSubstate;

extern const uint32_t * const DeviceID;

extern uint8_t LpUartRxBuffer[32];
extern uint8_t LpUartRxCounter;
extern LpUart LpUartTx;

extern uint8_t Uart2RxBuffer[CAMERA_BUFFER_SIZE];
extern uint8_t Uart2RxCounter;
extern TxUart2 Uart2Tx;

extern TimerQueueEntry timerSchedule[TIMER_SCHEDULE_SIZE];

extern int32_t tmp;
extern uint8_t uint8tmp;
extern uint32_t uint32tmp;

void init_Board_Peripherals(void);
void configureUart(USARTx base, uint32_t speed);


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
void boardWhiteLedOn(void);
void boardWhiteLedOff(void);
void boardWhiteLedToggle(void);
void boardRedLedOn(void);
void boardRedLedOff(void);
void boardRedLedToggle(void);
void driveGreenLed(uint8_t high_low);
void driveRedLed(uint8_t high_low);

void driveRfSwitch(uint8_t high_low);

void startLpUartMsg(uint8_t command);
void addToLpUartMsg(uint8_t byte);
void addToLpUartMsgAsHex(uint8_t byte);
void sendLpUartMessage(void);
void lpUartSendText(char *text);

void sendUart1Message(uint8_t *data, uint8_t length);					//send a command to GPS

void sendUart2Message(uint8_t *data, uint8_t length);

void spiAccessRegisters(uint8_t *data, uint8_t length);
void spiAccessRegisters2(uint8_t command, uint8_t *data, uint8_t length);

int i2cWrite(uint8_t *data, uint8_t length, uint8_t stop);
void i2cRead(uint8_t *data, uint8_t length);

//void startLpTimer(uint16_t timeout);
//void setTimer(uint32_t timeout, uint32_t event);
void setTimer(uint16_t timeout, uint16_t event);

void enterSleep(void);
void enterStandby(void);

void ErrorHandler(char *msg);

    
#ifdef __cplusplus
}
#endif

#endif // __STM32L051X_BOARD_H


