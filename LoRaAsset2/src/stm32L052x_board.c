//MCU programming primitives and board-specific hardware functions

#include <stm32L052x_board.h>
#include <string.h>


#define VECT_TAB_OFFSET  	0x00 		//Vector Table base offset field. This value must be a multiple of 0x200

#define I2C_REQUEST_READ	0x01										//added to address to indicate write vs. read


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
	ALARM_PRIORITY				= 6,
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

static const struct GPIO_Line inputs[] = {{GSINT1_PORT, GSINT1_BIT}, {GSINT2_PORT, GSINT2_BIT},
		{RADIO_DIO0_PORT, RADIO_DIO0_BIT}, {RADIO_DIO1_PORT, RADIO_DIO1_BIT}, {RADIO_DIO2_PORT, RADIO_DIO2_BIT}, {RADIO_DIO3_PORT, RADIO_DIO3_BIT}};

static const struct GPIO_Line outputs[] = {{WHITE_LED_PORT, WHITE_LED_BIT}, {AUX_RED_LED_PORT, AUX_RED_LED_BIT}, {AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT},
		{RADIO_RESET_PORT, RADIO_RESET_BIT}, {RADIO_NSS_PORT, RADIO_NSS_BIT}, {RADIO_MOSI_PORT, RADIO_MOSI_BIT}, {RADIO_SCK_PORT, RADIO_SCK_BIT},
		{RF_PA_HIGH_PWR_PORT, RF_PA_HIGH_PWR_BIT}, {RF_CPS_PORT, RF_CPS_BIT}, {RF_CSD_PORT, RF_CSD_BIT},
		{I2C1_SCL_PORT, I2C1_SCL_BIT}};


void SystemInit (void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);					//Enable power interface clock
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);			//Select voltage scale 1 (1.65V - 1.95V
	LL_RCC_HSI_Enable();												//Enable HSI
	while(0 == LL_RCC_HSI_IsReady());									//wait for HSI getting stable
	//LL_RCC_HSI_EnableOutput (void );
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);					//set HSI as system clock
	RCC->CFGR |= RCC_CFGR_STOPWUCK;										//HSI selection after wake-up from STOP

	//LL_RCC_SetAHBPrescaler
	//LL_RCC_SetAPB1Prescaler
	//LL_RCC_SetAPB2Prescaler
	//LL_RCC_ConfigMCO

//	RCC->CIER = 0x00000000;												//Disable all clock interrupts
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; 							//Vector Table location add offset address in Internal FLASH
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
	GPIO_ptr(base)->OSPEEDR &= ~(0b11 << (pin * 2));
//	GPIOx_OSPEEDR(base) |= (GPIO_PIN_SPEED_LOW << (pin * 2));			//set speed to lower setting to reduce electric noise
	GPIO_ptr(base)->OSPEEDR |= (GPIO_PIN_SPEED_WERY_HIGH << (pin * 2));	//just for SPI

	GPIO_ptr(base)->OTYPER &= ~(0b1 << pin);
	GPIO_ptr(base)->OTYPER |= (GPIO_PIN_PUSH_PULL << pin);				//configure output as Push-Pull

	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//reset previous value
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_OUTPUT << (pin * 2));		//set the pin as output

	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));						//neither low now high pull is active
}

//set alternate function for GPIO line
void configurePortPinAsAf(GPIO_Port base, GPIO_Number pin, GPIO_AF af)
{
	if(pin < GPIO_8) {													//set AF for the pin
		GPIO_ptr(base)->AFR[0] &= ~(0b1111 << (pin * 4));
		GPIO_ptr(base)->AFR[0] |= (af << (pin * 4));
	} else {
		GPIO_ptr(base)->AFR[1] &= ~(0b1111 << ((pin - 8) * 4));
		GPIO_ptr(base)->AFR[1] |= (af << ((pin - 8) * 4));
	}

	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//set AF pin mode
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_AF << (pin * 2));
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
void configurePortPinPullDown(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));
	GPIO_ptr(base)->PUPDR |= (GPIO_PIN_PULL_DOWN << (pin * 2));			//activate pull Down
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
void configurePortPinPullUp(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));
	GPIO_ptr(base)->PUPDR |= (GPIO_PIN_PULL_UP << (pin * 2));			//activate pull Up
}

//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
static inline void configurePortPinOpenDrain(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->OTYPER |= (GPIO_PIN_OPEN_DRAIN << pin);				//set pin as open-drain
}

static inline void configureAccelerometer(void)
{
	//Configure SCL Pin as : Alternate function, Open drain, Pull up
	configurePortPinPullUp(I2C1_SCL_PORT, I2C1_SCL_BIT);
	configurePortPinOpenDrain(I2C1_SCL_PORT, I2C1_SCL_BIT);
	configurePortPinAsAf(I2C1_SCL_PORT, I2C1_SCL_BIT, GPIO_AF_4);

	//Configure SDA Pin as : Alternate function, Open drain, Pull up
	configurePortPinPullUp(I2C1_SDA_PORT, I2C1_SDA_BIT);
	configurePortPinOpenDrain(I2C1_SDA_PORT, I2C1_SDA_BIT);
	configurePortPinAsAf(I2C1_SDA_PORT, I2C1_SDA_BIT, GPIO_AF_4);

	LL_I2C_Disable(I2C1);												//Disable I2C1 prior to modifying configuration registers

	//Timing register value is computed with the AN4235 xls file, fast Mode @400kHz with I2CCLK = 16MHz, rise time = 100ns, fall time = 10ns
	I2C1->TIMINGR = (uint32_t)0x00301019;	//0x000090f0;	//0x00B1112E;	//0x00B1112E;	0x00300619

//	LL_I2C_Enable(I2C1);
	accelerometerData.buffer[0] = AR_TEMP_CFG_REG;
	accelerometerData.buffer[1] = 0b11000000;							//enable temperature sensor
	accelerometerData.buffer[2] = 0b00100111;							//CTRL_REG1: HR / Normal / Low-power mode (10 Hz), Z, Y, X axis enabled
	AccelerometerWriteBytes(accelerometerData.buffer, 3);

	accelerometerData.buffer[0] = AR_CTRL_REG4;
	accelerometerData.buffer[1] = 0b10000000;							//set Block data update bit for temperature sensor
	AccelerometerWriteBytes(accelerometerData.buffer, 2);

	accelerometerData.buffer[0] = AR_CTRL_REG3;
//	accelerometerData.buffer[1] = 0b01100000;							//IA1 interrupt on INT1 pin, IA2 interrupt on INT1 pin
	accelerometerData.buffer[1] = 0b01000000;							//IA1 interrupt on INT1 pin
	AccelerometerWriteBytes(accelerometerData.buffer, 2);

//seems not needed
//	accelerometerData.buffer[0] = AR_CTRL_REG5;
//	//accelerometerData.buffer[1] = 0b00001110;							//Reboot memory content, Latch interrupt request on INT1_SRC (31h), with INT1_SRC (31h) register cleared by reading INT1_SRC (31h) itself
//	accelerometerData.buffer[1] = 0b00000000;
//	AccelerometerWriteBytes(accelerometerData.buffer, 2);

	accelerometerData.buffer[0] = AR_INT1_CFG;
//	accelerometerData.buffer[1] = 0b01111111;							//6-direction movement recognition
	accelerometerData.buffer[1] = 0b10010101;							//free fall movement recognition
	AccelerometerWriteBytes(accelerometerData.buffer, 2);

	accelerometerData.buffer[0] = AR_INT1_THS;
//	accelerometerData.buffer[1] = 0b00111000;							//interrupt threshold - sensitive
	accelerometerData.buffer[1] = 0b00101111;	//0b00100000;							//interrupt threshold
	accelerometerData.buffer[2] = 0b00000001;							//interrupt duration
	AccelerometerWriteBytes(accelerometerData.buffer, 3);

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);//Select Port A for EXTI0 line input source
	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM0);						//Rising trigger event configuration bit of line 0
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM0);								//enable EXTI0 line interrupt
	NVIC_SetPriority(EXTI0_1_IRQn, GPIO_PRIORITY);						//enable button interrupts in interrupt controller
	NVIC_EnableIRQ(EXTI0_1_IRQn);

/*	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE1);//Select Port A for EXTI1 line input source
	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM1);						//Rising trigger event configuration bit of line 2
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM1);								//enable EXTI2 line interrupt
	NVIC_SetPriority(EXTI0_1_IRQn, GPIO_PRIORITY);						//enable button interrupts in interrupt controller
	NVIC_EnableIRQ(EXTI0_1_IRQn);
*/
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

static void inline configureUart1(void)
{
	LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_HSI);			//would not wake up from Stop if clock source is not set to HSI
	configurePortPinAsAf(UART1_TX_PORT, UART1_TX_BIT, GPIO_AF_4);	 	//configure UART TX GPIO
	configurePortPinAsAf(UART1_RX_PORT, UART1_RX_BIT, GPIO_AF_4);		//configure UART RX GPIO
	configureUart(USART_1, US_USART_115200);							//configure UART communication parameters 115200
	NVIC_SetPriority(USART1_IRQn, USART_2_PRIORITY);					//enable UART2 interrupt in interrupt controller
	NVIC_EnableIRQ(USART1_IRQn);
}

static void inline configureUart2(void)
{
	LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_HSI);
	configurePortPinAsAf(UART2_TX_PORT, UART2_TX_BIT, GPIO_AF_4);		//configure UART TX GPIO
	configurePortPinAsAf(UART2_RX_PORT, UART2_RX_BIT, GPIO_AF_4);		//configure UART RX GPIO
	configureUart(USART_2, US_USART_14400);								//configure UART communication parameters 14400
	NVIC_SetPriority(USART2_IRQn, USART_2_PRIORITY);					//enable UART2 interrupt in interrupt controller
	NVIC_EnableIRQ(USART2_IRQn);
}

static void inline configureLpUart(void)
{
	LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_HSI);
	configurePortPinAsAf(LPUART_TX_PORT,LPUART_TX_BIT, GPIO_AF_4);		//configure UART TX GPIO
	configurePortPinAsAf(LPUART_RX_PORT,LPUART_RX_BIT, GPIO_AF_4);		//configure UART RX GPIO
//	configureUart(LPUART, 426667);										//configure LPUART communication parameters 9600
	configureUart(LPUART, US_LPUART_115200);							//configure LPUART communication parameters 115200
	NVIC_SetPriority(LPUART1_IRQn, USART_2_PRIORITY);					//enable LPUART interrupt in interrupt controller
	NVIC_EnableIRQ(LPUART1_IRQn);
}


static void inline configureSpi1(void)
{
	configurePortPinAsAf(RADIO_MOSI_PORT, RADIO_MOSI_BIT, GPIO_AF_0);	//master out slave in GPIO
	configurePortPinAsAf(RADIO_MISO_PORT, RADIO_MISO_BIT, GPIO_AF_0);	//master in slave out GPIO
	configurePortPinAsAf(RADIO_SCK_PORT, RADIO_SCK_BIT, GPIO_AF_0);		//clock GPIO
	configurePortPinPullDown(RADIO_SCK_PORT, RADIO_SCK_BIT);			//pull clock line down for CPOL=0
	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//deactivate NSS line

	SPI1->CR1 &= ~SPI_CR1_SPE;											//disable SPI1 in order to program
	SPI1->CR1 |= HALF_PERIPHERAL_CLOCK << SPI_CR1_BR_Pos				//8MHz from 16MHz peripheral clock
			 | SPI_CR1_MSTR												//SPI1 will be master
			 | SPI_CR1_SSM												//Software slave management
			 | SPI_CR1_SSI;												//internal slave select to avoid MODF error
	SPI1->CR1 |= SPI_CR1_SPE;											//enable SPI1
}


static inline void enableRTC(void)
{
	LL_PWR_EnableBkUpAccess();											//Enable access to the backup domain
	while(!LL_PWR_IsEnabledBkUpAccess());
	LL_RCC_LSE_Enable();												//Enable LSE
	while(!LL_RCC_LSE_IsReady());										//wait for LSE to become ready
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);					//set LSE as RTC clock
	LL_RCC_EnableRTC();													//at last, enable RTC

	LL_RTC_DisableWriteProtection(RTC);
//	LL_RTC_EnableShadowRegBypass(RTC);
	LL_RTC_DisableShadowRegBypass(RTC);

	//configure prescalers such that subseconds tick would be 1/1024th of a second i.e. ~1ms
	if((LL_RTC_GetAsynchPrescaler(RTC) != 31) || (LL_RTC_GetSynchPrescaler(RTC) != 1023))
	{
		LL_RTC_EnableInitMode(RTC);
		while(!LL_RTC_IsActiveFlag_INIT(RTC));
		LL_RTC_SetSynchPrescaler(RTC, (uint32_t)1023);					//ck_spre=ck_apre/(SYNC prediv + 1)
		LL_RTC_SetAsynchPrescaler(RTC, (uint32_t)31);					//ck_apre=LSEFreq/(ASYNC prediv + 1) = 1024Hz with LSEFreq=32768Hz
		LL_RTC_DisableInitMode(RTC);
	}

	//RTC alarm A configuration
	LL_RTC_ALMA_Disable(RTC);
	RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
	while(!LL_RTC_IsActiveFlag_ALRAW(RTC));
	LL_RTC_EnableIT_ALRA(RTC);											//enable Alarm A interrupt
//	LL_RTC_ALMA_Enable(RTC);											//enable Alarm A

	//RTC alarm B configuration
	LL_RTC_ALMB_Disable(RTC);
	RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);		//Clear the AlarmB interrupt pending bit
	while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
	LL_RTC_ALMB_SetMask(RTC, RTC_ALRMBR_MSK4 | RTC_ALRMBR_MSK3);		//trigger when minutes, seconds and subseconds match
	LL_RTC_ALMB_SetSubSecondMask(RTC, 10);								//0 - 1023, compare 10 bits of sub second value (1 bit ~= 1ms) for alarm event
	LL_RTC_EnableIT_ALRB(RTC);											//enable Alarm B interrupt
//	LL_RTC_ALMB_Enable(RTC);											//enable Alarm B

//	LL_RTC_EnableWriteProtection(RTC);

	//After a system reset, the software must wait until RSF is set before reading the RTC_SSR RTC_TR and RTC_DR
	while(!LL_RTC_IsActiveFlag_RS(RTC));

	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM17);						//connect RTC Alarm to EXTI line
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM17);

	NVIC_SetPriority(RTC_IRQn, ALARM_PRIORITY);							//enable RTC Alarms interrupts in interrupt controller
	NVIC_EnableIRQ(RTC_IRQn);
}


static inline void enableLPTim(void)
{
	LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSE);			//32.768kHz
	//LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV32);			//~1ms clock 1/1024s
	LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV8);
	LL_LPTIM_EnableTimeout(LPTIM1);										//enable wake-up
	LL_LPTIM_EnableIT_CMPM(LPTIM1);										//Enable Compare match interrupt

	LL_LPTIM_Enable(LPTIM1);											//Enable LPTIM
	LL_LPTIM_SetAutoReload(LPTIM1, 0xFFFF);								//Load the period value has to be strictly bigger than compare value

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_29);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_29);						//connect LPTIM to EXTI line

	NVIC_SetPriority(LPTIM1_IRQn, LPTIMER_PRIORITY);

	LL_LPTIM_SetAutoReload(LPTIM1, 3);
	LL_LPTIM_SetCompare(LPTIM1,2);
	LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS);

	NVIC_EnableIRQ(LPTIM1_IRQn);
}


static inline void configureRadio(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE2);//Connect DIO0 pin to EXTI_2 line
	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM2);						//configure EXTI_2 for rising trigger interrupt
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM2);								//enable line 2 interrupt in EXTI

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE5);//Connect DIO3 pin to EXTI_5 line
	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM5);						//configure EXTI_5 for rising trigger interrupt
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM5);								//enable line 5 interrupt in EXTI
	NVIC_SetPriority(EXTI2_3_IRQn, GPIO_PRIORITY);						//enable the EXTI lines interrupt in interrupt controller
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, GPIO_PRIORITY);						//enable the EXTI lines interrupt in interrupt controller
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);						//turn on the radio chip TODO: remove
}


void init_Board_Peripherals(void)
{
	//enable peripherals
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);					//enable port A
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);					//enable port B
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);					//enable port C
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);				//enable system configuration or writing to SYSCFG has no effect
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);				//enable UART1 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);				//enable UART2 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1);				//enable LPUART1 peripheral
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);					//enable SPI1 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);				//enable low-power timer
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);					//Enable the peripheral clock for I2C1

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);					//enable debug interface
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_DBGMCU);
	LL_DBGMCU_EnableDBGStopMode();
	LL_DBGMCU_EnableDBGStandbyMode();
	LL_DBGMCU_ABP1_GRP1_FreezePeriph(LL_DBGMCU_ABP1_GRP1_RTC_STOP | LL_DBGMCU_ABP1_GRP1_LPTIM1_STOP | LL_DBGMCU_ABP1_GRP1_I2C1_STOP);

	//configure inputs
	for(tmp = 0; tmp < sizeof(inputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsInput(inputs[tmp].port, inputs[tmp].bit);

	//configure outputs
	setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);

	for(tmp = 0; tmp < sizeof(outputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsOutput(outputs[tmp].port, outputs[tmp].bit);

	configureUart1();													//configure command interface

	configureUart2();													//configure camera interface

	configureLpUart();													//configure GPS interface

	configureSpi1();													//configure radio communication interface

    enableRTC();														//start RTC, clock source is actually set for the first time only, then proper RTC domain reset is needed to change it

	enableLPTim();														//LSE is enabled by enableRTC();

	configureAccelerometer();

	configureRadio();													//connect radio interrupts and enable the radio

	sendUart1Message("$PMTK161,0*28\r\n", 15);							//put GPS into low-power mode
}


//Timeout range is from 1 to FFFF 1/1024s => ~ 1ms - 1min, event is the flag to be set when timer expires
void setTimer(uint16_t timeout, uint16_t event)
{
	uint32_t i;
//	uint16_t delta = 0;

	NVIC_DisableIRQ(LPTIM1_IRQn);

	//find a place in scheduling table to insert a new event into
	for(i = 0; i < TIMER_SCHEDULE_SIZE; i++)
		if(timerSchedule[i].event == event)
			break;

	if(TIMER_SCHEDULE_SIZE == i)											//no such event was previously scheduled, find first empty entry for it
		for(i = 0; i < TIMER_SCHEDULE_SIZE; i++)
			if(0 == timerSchedule[i].event)
				break;		  												//empty entry should always exist so skipping check for the end here

	//here we have a position to insert new event into
	timerSchedule[i].event = event;

	//now let's see about the timing
	if (TimerCounter) {														//this means timer is running and counter register must be less than compare register

		if (timeout >= TimerCounter) {
			timerSchedule[i].timeout = timeout - TimerCounter;				//convert the timeout to timer terms
		} else {															//the new event will be first to expire
			timeout = TimerCounter - timeout;								//now use timeout as delta holder, - other scheduled events timeouts need to be increased by this much
			TimerCounter -= timeout;										//schedule the new event with original timout value
			timerSchedule[i].timeout = 0 - timeout;							//to become 0 after following expire time updating loop
			for (i = 0; i < TIMER_SCHEDULE_SIZE; i++)
				timerSchedule[i].timeout += timeout;
		}
	} else {																//scheduling table must be empty, the easy case
		timerSchedule[i].timeout = 0;
		TimerCounter = timeout;
	}

	HardwareEvents &= ~event;

	NVIC_EnableIRQ(LPTIM1_IRQn);
}


void startLpUartMsg(uint8_t command)
{
	while(HardwareEvents & HWE_LPUART_TRANSMITTING);					//make sure not to screw up a message that may be currently in progress
	LpUartTx.size = 1;													//reserve space for message length
	LpUartTx.data[LpUartTx.size++] = command;
}

void addToLpUartMsg(uint8_t byte)
{
	if(LpUartTx.size >= sizeof(LpUartTx.data))
		ErrorHandler("addToUart2Msg() error");
	else
		LpUartTx.data[LpUartTx.size++] = byte;
}

void sendLpUartMessage(void)
{
	HardwareEvents |= HWE_LPUART_TRANSMITTING;							//uart transmission is in progress
	LpUartTx.data[0] = LpUartTx.size;									//number of bytes in a message including first length byte
	LpUartTx.counter = 0;												//TX buffer should be filled and size properly set
	USART_ptr(LPUART)->CR1 |= USART_CR1_TCIE;							//enable transfer complete interrupt initiating interrupt - driven transmission
}

void lpUartSendText(char *text)
{
	while(HardwareEvents & HWE_LPUART_TRANSMITTING);
	startLpUartMsg(RC_TEXT);

	LpUartTx.size = strlen(text);
	if(LpUartTx.size > sizeof(LpUartTx.data) - 2)
		LpUartTx.size = sizeof(LpUartTx.data) - 2;
	memcpy(LpUartTx.data + 2, text, LpUartTx.size);
	LpUartTx.size += 2;
	sendLpUartMessage();
}

void sendUart2Message(uint8_t *data, uint8_t length)
{
	while(HardwareEvents & HWE_UART2_TRANSMITTING);
	HardwareEvents |= HWE_UART2_TRANSMITTING;							//uart transmission is in progress
	if(length > sizeof(Uart2Tx.data))
		length = sizeof(Uart2Tx.data);
	memmove(Uart2Tx.data, data, length);
	Uart2Tx.size = length;												//number of bytes in a message including first length byte
	Uart2Tx.counter = 0;												//TX buffer should be filled and size properly set
	USART_ptr(USART_2)->CR1 |= USART_CR1_TCIE;							//enable transfer complete interrupt initiating interrupt - driven transmission
}

void sendUart1Message(uint8_t *data, uint8_t length)					//send a command to GPS
{
	while(HardwareEvents & HWE_UART1_TRANSMITTING);
	HardwareEvents |= HWE_UART1_TRANSMITTING;							//uart transmission is in progress
	if(length > sizeof(Uart1Tx.data))
		length = sizeof(Uart1Tx.data);
	memmove(Uart1Tx.data, data, length);
	Uart1Tx.size = length;												//number of bytes in a message including first length byte
	Uart1Tx.counter = 0;												//TX buffer should be filled and size properly set
	USART_ptr(USART_1)->CR1 |= USART_CR1_TCIE;							//enable transfer complete interrupt initiating interrupt - driven transmission
}

void spiAccessRegisters(uint8_t *data, uint8_t length)
{
	resetPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);						//start SPI transaction - assert data frame start

	(void)SPI1->DR;														//read and discard RX buffer to clear RXNE, just in case
	while(length--)
	{
		SPI1->DR = *data;												//write a data byte
		while(0 == (SPI1->SR & SPI_SR_RXNE));							//wait until reply is ready
		*(data++) = SPI1->DR;											//save the reply. * Address byte will be overwritten with 0
	}

	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//end SPI access - deactivate NSS line
}

void spiAccessRegisters2(uint8_t command, uint8_t *data, uint8_t length)
{
	resetPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);						//assert data frame start

	(void)SPI1->DR;														//read and discard RX buffer to clear RXNE, just in case

	SPI1->DR = command;													//send the address with the Write bit indicator
	while(0 == (SPI1->SR & SPI_SR_RXNE));								//wait until address transmits
	(void)SPI1->DR;														//discard dummy return, otherwise next byte returned would be lost

	while(length--)
	{
		SPI1->DR = *data;												//write a data byte
		while(0 == (SPI1->SR & SPI_SR_RXNE));							//wait until reply is ready
		*(data++) = SPI1->DR;											//save the reply
	}

	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//deactivate NSS line
}


//the stop parameter should be zero if i2cRead is called right after i2cWrite, to generate restart condition
int i2cWrite(uint8_t *data, uint8_t length, uint8_t stop)
{
	LL_I2C_SetSlaveAddr(I2C1, ACCELEROMETER_I2C_ADDRESS);
	LL_I2C_SetTransferSize(I2C1, length);
	LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
	LL_I2C_GenerateStartCondition(I2C1);								//Initiate a Start condition to the Slave device

	while(!LL_I2C_IsActiveFlag_TC(I2C1)) {								//wait until length bytes are transmitted
		if(LL_I2C_IsActiveFlag_NACK(I2C1))
			return 0;
//		if(LL_I2C_IsActiveFlag_TXE(I2C1))
		if(LL_I2C_IsActiveFlag_TXIS(I2C1))
			LL_I2C_TransmitData8(I2C1, *data++);
	}

	if(stop) {
		LL_I2C_GenerateStopCondition(I2C1);								//Generate Stop condition
		while(!LL_I2C_IsActiveFlag_STOP(I2C1));							//wait until the stop appears on I2C bus
	}

	return 1;
}


void i2cRead(uint8_t *data, uint8_t length)
{
	LL_I2C_SetSlaveAddr(I2C1, ACCELEROMETER_I2C_ADDRESS);
	LL_I2C_SetTransferSize(I2C1, length);
	LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_READ);
	LL_I2C_GenerateStartCondition(I2C1);								//Initiate a Start condition to the Slave device

	//here master is in receive mode activated by I2C_REQUEST_READ bit
	while(length)														//receive data
		if(LL_I2C_IsActiveFlag_RXNE(I2C1)) {							//Check RXNE flag value in ISR register and read next data byte
			*data++ = LL_I2C_ReceiveData8(I2C1);						//Read data in Receive Data register. RXNE flag is cleared by reading data from RXDR register
			length--;
		}

	LL_I2C_GenerateStopCondition(I2C1);									//and Stop condition after receiving next byte
	while(!LL_I2C_IsActiveFlag_STOP(I2C1));								//wait until the stop appears on I2C bus
}



inline void boardGreenLedOn(void)
{
	setPortBit(AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT);
}

inline void boardGreenLedOff(void)
{
	resetPortBit(AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT);
}

void boardGreenLedToggle(void)
{
	if(readPortBit(AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT))
		boardGreenLedOff();
	else
		boardGreenLedOn();
}

inline void boardWhiteLedOn(void)
{
	setPortBit(WHITE_LED_PORT, WHITE_LED_BIT);
}

inline void boardWhiteLedOff(void)
{
	resetPortBit(WHITE_LED_PORT, WHITE_LED_BIT);
}

void boardWhiteLedToggle(void)
{
	if(readPortBit(WHITE_LED_PORT, WHITE_LED_BIT))
		boardWhiteLedOff();
	else
		boardWhiteLedOn();
}

inline void boardRedLedOn(void)
{
	setPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT);
}

inline void boardRedLedOff(void)
{
	resetPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT);
}

void boardRedLedToggle(void)
{
	if(readPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT))
		boardRedLedOff();
	else
		boardRedLedOn();
}


void driveGreenLed(uint8_t high_low)
{
	if(high_low)
		boardGreenLedOn();
	else
		boardGreenLedOff();
}

void driveRedLed(uint8_t high_low)
{
	if(high_low)
		boardRedLedOn();
	else
		boardRedLedOff();
}

void driveRfSwitch(uint8_t switches)
{
	if(switches & 0b100)												//sets RF output: Low - RFO_HF, High - PA_BOOST
		setPortBit(RF_PA_HIGH_PWR_PORT, RF_PA_HIGH_PWR_BIT);
	else
		resetPortBit(RF_PA_HIGH_PWR_PORT, RF_PA_HIGH_PWR_BIT);

	if(switches & 0b010)
		setPortBit(RF_CPS_PORT, RF_CPS_BIT);							//sets gain control in receive mode: Low - receive low gain, High - receive high gain
	else
		resetPortBit(RF_CPS_PORT, RF_CPS_BIT);

	if(switches & 0b001)												//Shutdown control: Low - turned off, High - RX or TX is enabled
		setPortBit(RF_CSD_PORT, RF_CSD_BIT);
	else
		resetPortBit(RF_CSD_PORT, RF_CSD_BIT);
}


void enterSleep(void)
{
	LL_PWR_ClearFlag_WU();												//ensure that the WUF  wake up flag is cleared
	LL_LPM_EnableDeepSleep();											//Set SLEEPDEEP bit of Cortex System Control Register
	__WFI();															//Request Wait For Interrupt
	//TODO: is it necessary?
//	CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);							//Reset SLEEPDEEP bit after exiting from Stop
}


void enterStandby(void)
{
	lpUartSendText("Bye, going to StandBy");
	while(HardwareEvents & HWE_LPUART_TRANSMITTING);					//wait until the message transmits

	LL_PWR_ClearFlag_WU();
	LL_PWR_ClearFlag_SB();
//	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
	LL_LPM_EnableDeepSleep();
	LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

	__SEV();
	__WFE();
	__WFE();
}


//This function is executed in case of error occurrence
void ErrorHandler(char *msg)
{
	boardRedLedOn();
	lpUartSendText(msg);
	while(1);
}

#ifdef  USE_FULL_ASSERT
//Reports the name of the source file and the source line number where the assert_param error has occurred
void assert_failed(uint8_t* file, uint32_t line)
{
//User can add his own implementation to report the file name and line number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	while (1);
}
#endif
