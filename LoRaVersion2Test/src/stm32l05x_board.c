//MCU programming primitives and board-specific hardware functions

#include "stm32l05x_board.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include <string.h>


#define VECT_TAB_OFFSET  0x00 // Vector Table base offset field. This value must be a multiple of 0x200

typedef enum
{
	GPIO_AF_0,
	GPIO_AF_1,
	GPIO_AF_2,
	GPIO_AF_3,
	GPIO_AF_4,
	GPIO_AF_5,
	GPIO_AF_6,
	GPIO_AF_7,
}GPIO_AF;

typedef enum
{
	GPIO_PIN_MODE_INPUT			= 0b00,
	GPIO_PIN_MODE_OUTPUT		= 0b01,
	GPIO_PIN_MODE_AF			= 0b10,
	GPIO_PIN_MODE_ANALOG		= 0b11,
} GPIO_Pin_Mode;

typedef enum
{
	GPIO_PIN_PUSH_PULL			= 0b0,
	GPIO_PIN_OPEN_DRAIN			= 0b1,
} GPIO_Pin_Output_Type;

typedef enum
{
	GPIO_PIN_SPEED_WERY_LOW		= 0b00,
	GPIO_PIN_SPEED_LOW			= 0b01,
	GPIO_PIN_SPEED_MEDIUM		= 0b10,
	GPIO_PIN_SPEED_WERY_HIGH	= 0b11,
} GPIO_Pin_Speed;

typedef enum
{
	GPIO_PIN_NO_PULLS			= 0b00,
	GPIO_PIN_PULL_UP			= 0b01,
	GPIO_PIN_PULL_DOWN			= 0b10,
	GPIO_PIN_RESERVED			= 0b11,
} GPIO_Pin_PullUpDown;


typedef enum
{
	ALARM_PRIORITY				= 0,
	LPTIMER_PRIORITY,
	USART_2_PRIORITY,
	GPIO_PRIORITY				= USART_2_PRIORITY,
} Interrupt_Priorities;

typedef enum
{
	HALF_PERIPHERAL_CLOCK		= 0b000,
	QUARTER_PERIPHERAL_CLOCK	= 0b001,
	ONE8TH_PERIPHERAL_CLOCK		= 0b010,
	ONE16TH_PERIPHERAL_CLOCK	= 0b011,
	ONE32TH_PERIPHERAL_CLOCK	= 0b100,
	ONE64TH_PERIPHERAL_CLOCK	= 0b101,
	ONE128TH_PERIPHERAL_CLOCK	= 0b110,
	ONE256TH_PERIPHERAL_CLOCK	= 0b111,
} SPI_Speed;

typedef enum
{
	RADIO_READ					= 0x00,
	RADIO_WRITE					= 0x80
} Radio_Read_Write;


struct GPIO_Line
{
	GPIO_Port port;
	GPIO_Number bit;
};


static const struct GPIO_Line outputs[] = {{LED_PORT, LED_BIT}};


void SystemInit (void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);					//Enable power interface clock
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);			//Select voltage scale 1 (1.65V - 1.95V)
	LL_RCC_HSI_Enable();												//Enable HSI
	while(0 == LL_RCC_HSI_IsReady());									//wait for HSI getting stable
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);					//set HSI as system clock
	RCC->CFGR |= RCC_CFGR_STOPWUCK;										//HSI selection after wake-up from STOP

	RCC->CIER = 0x00000000;												//Disable all clock interrupts
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; 							//Vector Table location add offset address in Internal FLASH
}

static inline void setPortBit(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->BSRR = (0x01 << pin);
}

static inline void resetPortBit(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->BRR = (0x01 << pin);
}

inline uint32_t readPortBit(GPIO_Port base, GPIO_Number pin)
{
	return (GPIO_ptr(base)->IDR & (0x01 << pin));
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
void configurePortPinAsInput(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//reset previous value
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_INPUT << (pin * 2));		//set the pin as input

	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));
	GPIO_ptr(base)->PUPDR |= (GPIO_PIN_NO_PULLS << (pin * 2));			//neither low now high pull is active
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
void configurePortPinAsOutput(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//reset previous value
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_OUTPUT << (pin * 2));		//set the pin as output

	GPIO_ptr(base)->OTYPER &= ~(0b1 << pin);
	GPIO_ptr(base)->OTYPER |= (GPIO_PIN_PUSH_PULL << pin);				//configure output as Push-Pull

	GPIO_ptr(base)->OSPEEDR &= ~(0b11 << (pin * 2));
//	GPIOx_OSPEEDR(base) |= (GPIO_PIN_SPEED_LOW << (pin * 2));			//set speed to lower setting to reduce electric noise
	GPIO_ptr(base)->OSPEEDR |= (GPIO_PIN_SPEED_WERY_HIGH << (pin * 2));	//just for SPI

	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));
//	GPIOx_PUPDR(base) |= (GPIO_PIN_NO_PULLS << (pin * 2));				//neither low now high pull is active
//	GPIOx_PUPDR(base) |= (GPIO_PIN_PULL_UP << (pin * 2));				//pull Up just in case
}

//set alternate function for GPIO line
void configurePortPinAsAf(GPIO_Port base, GPIO_Number pin, GPIO_AF af)
{
	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//set AF pin mode
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_AF << (pin * 2));

	if(pin < GPIO_8)													//set AF for the pin
	{
		GPIO_ptr(base)->AFR[0] &= ~(0b1111 << (pin * 4));
		GPIO_ptr(base)->AFR[0] |= (af << (pin * 4));
	} else
	{
		GPIO_ptr(base)->AFR[1] &= ~(0b1111 << ((pin - 8) * 4));
		GPIO_ptr(base)->AFR[1] |= (af << ((pin - 8) * 4));
	}
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
void configurePortPinPullDown(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));
	GPIO_ptr(base)->PUPDR |= (GPIO_PIN_PULL_DOWN << (pin * 2));			//activate pull Down
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
inline void configurePortPinOpenDrain(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->OTYPER |= (GPIO_PIN_OPEN_DRAIN << pin);				//set pin as open-drain
}


void configureUart(USARTx base, uint32_t speed)
{
	USART_ptr(base)->CR1 &=  ~USART_CR1_UE;								//disable UART in order to program
//	USART_ptr(base)->BRR = 0x8A;										//115200 baud from 16MHz HSI, should it be 8B? guess was tried with experimental clocks match
	USART_ptr(base)->BRR = speed;										//baud rate from 16MHz HSI (16MHz / (16 * div), div has last four bits as fractional part expressed in 1/16th)
	USART_ptr(base)->CR1 |= USART_CR1_UESM | USART_CR1_RXNEIE;			//Enable UART in STOP Mode and interrupt on character reception, also used for wake up from Stop Mode
//	USART_ptr(base)->CR3 |= USART_CR3_UCESM;							//Clock Enable in Stop mode *feature removed in 2015
	USART_ptr(base)->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;	//enable UART, UART receiver and UART transmitter
}

/*
static void inline configureUart1(void)
{
	configurePortPinAsAf(UART1_TX_PORT, UART1_TX_BIT, GPIO_AF_4);		//configure UART TX GPIO
	configurePortPinAsAf(UART1_RX_PORT, UART1_RX_BIT, GPIO_AF_4);		//configure UART RX GPIO
	configureUart(USART_1);												//configure UART communication parameters
}
*/

static void inline configureLpUart(void)
{
	LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_HSI);
	configurePortPinAsAf(LPUART_TX_PORT, LPUART_TX_BIT, GPIO_AF_4);		//configure UART TX GPIO
	configurePortPinAsAf(LPUART_RX_PORT, LPUART_RX_BIT, GPIO_AF_4);		//configure UART RX GPIO
//	configureUart(LPUART, 426667);										//configure LPUART communication parameters 9600
	configureUart(LPUART, US_LPUART_115200);							//configure LPUART communication parameters 115200
	NVIC_SetPriority(LPUART1_IRQn, USART_2_PRIORITY);					//enable LPUART interrupt in interrupt controller
	NVIC_EnableIRQ(LPUART1_IRQn);
}

static inline void enableLPTim(void)
{
	LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSE);
	LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV32);			//~1ms clock
	LL_LPTIM_EnableTimeout(LPTIM1);										//enable wake-up
	LL_LPTIM_EnableIT_CMPM(LPTIM1);										//Enable Compare match interrupt
	LL_LPTIM_Enable(LPTIM1);											//Enable LPTIM

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_29);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_29);						//connect LPTIM to EXTI line

	NVIC_SetPriority(LPTIM1_IRQn, LPTIMER_PRIORITY);
	NVIC_EnableIRQ(LPTIM1_IRQn);

	LL_LPTIM_SetAutoReload(LPTIM1, 0xFFFF);								//Load the period value has to be strictly bigger than compare value
}


void init_Board_Peripherals(void)
{
	//enable peripherals
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);					//enable port A
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);					//enable port B
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);					//enable port C
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);				//enable system configuration or writing to SYSCFG has no effect
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);				//enable UART2 peripheral
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);				//enable UART1 peripheral
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);					//enable SPI1 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);				//enable low-power timer
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1);

	LL_PWR_EnableBkUpAccess();											//Enable access to the backup domain
	while(!LL_PWR_IsEnabledBkUpAccess());
	LL_RCC_LSE_Enable();												//Enable LSE
	while(!LL_RCC_LSE_IsReady());										//wait for LSE to become ready
	LL_RTC_DisableWriteProtection(RTC);

	//configure outputs
	for(tmp = 0; tmp < sizeof(outputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsOutput(outputs[tmp].port, outputs[tmp].bit);

	//configure command interface
//	configureUart1();													//enable and configure UART1

	configureLpUart();

	enableLPTim();
}

//starts one-shot timer to timeout milliseconds
void startLpTimer(uint16_t timeout)
{
	LL_LPTIM_SetCompare(LPTIM1, timeout);								//Load the Timeout value
	LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT);		//LL_LPTIM_OPERATING_MODE_CONTINUOUS
}



void uart1Transmit(uint8_t *data, uint32_t length)
{
	while(length--)
	{
		while(!(USART_ptr(USART_1 )->ISR & USART_ISR_TXE));
		USART_ptr(USART_1 )->TDR = *(data++);
	}
}


void sendLpUartMessage(uint8_t *data, uint8_t length)
{
	while(HardwareEvents & HWE_LPUART_TRANSMITTING);
	HardwareEvents |= HWE_LPUART_TRANSMITTING;							//uart transmission is in progress
	if(length > sizeof(LpUartTx.data))
		length = sizeof(LpUartTx.data);
	memmove(LpUartTx.data, data, length);
	LpUartTx.size = length;												//number of bytes in a message including first length byte
	LpUartTx.counter = 0;												//TX buffer should be filled and size properly set
	USART_ptr(LPUART)->CR1 |= USART_CR1_TCIE;							//enable transfer complete interrupt initiating interrupt - driven transmission
}


static inline void boardLedOn(void)
{
	setPortBit(LED_PORT, LED_BIT);
}

void boardLedOff(void)
{
	resetPortBit(LED_PORT, LED_BIT);
}

void boardLedToggle(void)
{
	if(readPortBit(LED_PORT, LED_BIT))
		boardLedOff();
	else
		boardLedOn();
}


