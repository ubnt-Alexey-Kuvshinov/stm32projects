//MCU programming primitives and board-specific hardware functions

#include <stm32l152x_board.h>
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
	GPIO_AF_8,
	GPIO_AF_9,
	GPIO_AF_10,
	GPIO_AF_11,
	GPIO_AF_12,
	GPIO_AF_13,
	GPIO_AF_14,
	GPIO_AF_15,
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

static const struct GPIO_Line inputs[] = {{BUTTON_PORT, BUTTON_BIT}, /*{AUX_BUTTON_PORT, AUX_BUTTON_BIT},*/
		{RADIO_DIO0_PORT, RADIO_DIO0_BIT}, {RADIO_DIO1_PORT, RADIO_DIO1_BIT}, {RADIO_DIO2_PORT, RADIO_DIO2_BIT},
		{RADIO_DIO3_PORT, RADIO_DIO3_BIT}, {RADIO_DIO4_PORT, RADIO_DIO4_BIT}, {RADIO_DIO5_PORT, RADIO_DIO5_BIT},
		/*{RADIO_MISO_PORT, RADIO_MISO_BIT}*/};

static const struct GPIO_Line outputs[] = {{AUX_RED_LED_PORT, AUX_RED_LED_BIT}, {AUX_YELLOW_LED_PORT, AUX_YELLOW_LED_BIT}, {AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT},
		{RADIO_RESET_PORT, RADIO_RESET_BIT}, {RADIO_NSS_PORT, RADIO_NSS_BIT},
		{RADIO_MOSI_PORT, RADIO_MOSI_BIT}, {RADIO_SCK_PORT, RADIO_SCK_BIT},
		{RF_SWITCH_CONTROL_1_PORT, RF_SWITCH_CONTROL_1_BIT}, {RF_SWITCH_CONTROL_2_PORT, RF_SWITCH_CONTROL_2_BIT}, {RF_PA_HIGH_PWR_PORT, RF_PA_HIGH_PWR_BIT},
		{GPS_ENABLE_PORT, GPS_ENABLE_BIT}};


void SystemInit (void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);					//Enable power interface clock
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);			//Select voltage scale 1 (1.65V - 1.95V
	LL_RCC_HSI_Enable();												//Enable HSI
	while(0 == LL_RCC_HSI_IsReady());									//wait for HSI getting stable
	//LL_RCC_HSI_EnableOutput (void );
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);					//set HSI as system clock

	//LL_RCC_SetAHBPrescaler
	//LL_RCC_SetAPB1Prescaler
	//LL_RCC_SetAPB2Prescaler
	//LL_RCC_ConfigMCO

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
	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//reset previous value
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_OUTPUT << (pin * 2));		//set the pin as output

	GPIO_ptr(base)->OTYPER &= ~(0b1 << pin);
	GPIO_ptr(base)->OTYPER |= (GPIO_PIN_PUSH_PULL << pin);				//configure output as Push-Pull

	GPIO_ptr(base)->OSPEEDR &= ~(0b11 << (pin * 2));
//	GPIOx_OSPEEDR(base) |= (GPIO_PIN_SPEED_LOW << (pin * 2));			//set speed to lower setting to reduce electric noise
	GPIO_ptr(base)->OSPEEDR |= (GPIO_PIN_SPEED_WERY_HIGH << (pin * 2));	//just for SPI

	GPIO_ptr(base)->PUPDR &= ~(0b11 << (pin * 2));						//neither low now high pull is active
}

//set alternate function for GPIO line
void configurePortPinAsAf(GPIO_Port base, GPIO_Number pin, GPIO_AF af)
{
	GPIO_ptr(base)->MODER &= ~(0b11 << (pin * 2));						//set AF pin mode
	GPIO_ptr(base)->MODER |= (GPIO_PIN_MODE_AF << (pin * 2));

	if(pin < GPIO_8) {													//set AF for the pin
		GPIO_ptr(base)->AFR[0] &= ~(0b1111 << (pin * 4));
		GPIO_ptr(base)->AFR[0] |= (af << (pin * 4));
	} else {
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
static inline void configurePortPinOpenDrain(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->OTYPER |= (GPIO_PIN_OPEN_DRAIN << pin);				//set pin as open-drain
}

static inline void configureButton(void)
{
																		//Select Port A for EXTI0 line input source (default)
	EXTI->IMR |= EXTI_IMR_IM0;											//enable EXTI0 line interrupt
	EXTI->RTSR |= EXTI_RTSR_RT0;										//Rising trigger event configuration bit of line 0
	EXTI->FTSR |= EXTI_FTSR_FT0;										//Falling trigger event configuration bit of line 0
	NVIC_SetPriority(EXTI0_IRQn, GPIO_PRIORITY);						//enable button interrupts in interrupt controller
	NVIC_EnableIRQ(EXTI0_IRQn);
}


void configureUart(USARTx base, uint16_t speed)
{
	USART_ptr(base)->CR1 &=  ~USART_CR1_UE;								//disable UART in order to program
//	USART_ptr(base)->BRR = 0x8A;										//115200 baud from 16MHz HSI, should it be 8B? guess was tried with experimental clocks match
	USART_ptr(base)->BRR = speed;										//baud rate from 16MHz HSI (16MHz / (16 * div), div has last four bits as fractional part expressed in 1/16th)
//	USART_ptr(base)->CR1 |= USART_CR1_RXNEIE;							//Enable UART interrupt on character reception
	USART_ptr(base)->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;	//enable UART, UART receiver and UART transmitter
}

static void inline configureUart1(void)
{
	configurePortPinAsAf(UART1_TX_PORT, UART1_TX_BIT, GPIO_AF_7);		//configure UART TX GPIO
	configurePortPinAsAf(UART1_RX_PORT, UART1_RX_BIT, GPIO_AF_7);		//configure UART RX GPIO
	configureUart(USART_1, 0x683);										//configure UART communication parameters 9600
	NVIC_SetPriority(USART1_IRQn, USART_2_PRIORITY);					//enable UART2 interrupt in interrupt controller
	NVIC_EnableIRQ(USART1_IRQn);
}

static void inline configureUart2(void)
{
	configurePortPinAsAf(UART2_TX_PORT, UART2_TX_BIT, GPIO_AF_7);		//configure UART TX GPIO
	configurePortPinAsAf(UART2_RX_PORT, UART2_RX_BIT, GPIO_AF_7);		//configure UART RX GPIO
	configureUart(USART_2, 0x8B);										//configure UART communication parameters
	NVIC_SetPriority(USART2_IRQn, USART_2_PRIORITY);					//enable UART2 interrupt in interrupt controller
	NVIC_EnableIRQ(USART2_IRQn);
}

static void inline configureSpi2(void)
{
	configurePortPinAsAf(RADIO_MOSI_PORT, RADIO_MOSI_BIT, GPIO_AF_5);	//master out slave in GPIO
	configurePortPinAsAf(RADIO_MISO_PORT, RADIO_MISO_BIT, GPIO_AF_5);	//master in slave out GPIO
	configurePortPinAsAf(RADIO_SCK_PORT, RADIO_SCK_BIT, GPIO_AF_5);		//clock GPIO
	configurePortPinPullDown(RADIO_SCK_PORT, RADIO_SCK_BIT);			//pull clock line down for CPOL=0
	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//deactivate NSS line

	SPI2->CR1 &= ~SPI_CR1_SPE;											//disable SPI2 in order to program
	SPI2->CR1 |= HALF_PERIPHERAL_CLOCK << SPI_CR1_BR_Pos				//8MHz from 16MHz peripheral clock
			 | SPI_CR1_MSTR												//SPI2 will be master
			 | SPI_CR1_SSM												//Software slave management
			 | SPI_CR1_SSI;												//internal slave select to avoid MODF error
	SPI2->CR1 |= SPI_CR1_SPE;											//enable SPI2
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
	LL_RTC_EnableIT_ALRA(RTC);										//enable Alarm A interrupt
//	LL_RTC_ALMA_Enable(RTC);										//enable Alarm A

	//RTC alarm B configuration
	LL_RTC_ALMB_Disable(RTC);
	RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);		//Clear the AlarmB interrupt pending bit
	while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
	LL_RTC_ALMB_SetMask(RTC, RTC_ALRMBR_MSK4 | RTC_ALRMBR_MSK3);	//trigger when minutes, seconds and subseconds match
	LL_RTC_ALMB_SetSubSecondMask(RTC, 10);							//0 - 1023, compare 10 bits of sub second value (1 bit ~= 1ms) for alarm event
	LL_RTC_EnableIT_ALRB(RTC);										//enable Alarm B interrupt
//	LL_RTC_ALMB_Enable(RTC);										//enable Alarm B

//	LL_RTC_EnableWriteProtection(RTC);

	//After a system reset, the software must wait until RSF is set before reading the RTC_SSR RTC_TR and RTC_DR
	while(!LL_RTC_IsActiveFlag_RS(RTC));

	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM17);						//connect RTC Alarm to EXTI line
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM17);

	NVIC_SetPriority(RTC_Alarm_IRQn, ALARM_PRIORITY);					//enable RTC Alarms interrupts in interrupt controller
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
}


static inline void enableLPTim(void)
{
/*
	LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSE);
	LL_LPTIM_SetPrescaler(LPT_ptr, LL_LPTIM_PRESCALER_DIV32);			//~1ms clock
	LL_LPTIM_EnableTimeout(LPT_ptr);									//enable wake-up
	LL_LPTIM_EnableIT_CMPM(LPT_ptr);									//Enable Compare match interrupt
	LL_LPTIM_Enable(LPT_ptr);											//Enable LPTIM

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_29);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_29);						//connect RTC Alarm to EXTI line

	NVIC_SetPriority(LPTIM1_IRQn, LPTIMER_PRIORITY);
	NVIC_EnableIRQ(LPTIM1_IRQn);

	LL_LPTIM_SetAutoReload(LPT_ptr, 0xFFFF);							//Load the period value has to be strictly bigger than compare value
//	LL_LPTIM_SetCompare(LPT_ptr, 100);									//Load the Timeout value
//	LL_LPTIM_StartCounter(LPT_ptr, LL_LPTIM_OPERATING_MODE_CONTINUOUS);	//LL_LPTIM_OPERATING_MODE_ONESHOT

 */
}

static inline void configureI2C1(void)
{
	//Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up
	configurePortPinAsAf(I2C1_SCL_PORT, I2C1_SCL_BIT, GPIO_AF_4);
	configurePortPinOpenDrain(I2C1_SCL_PORT, I2C1_SCL_BIT);
//	  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
//	  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
//	  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
//	  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
//	  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

	//Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up
	configurePortPinAsAf(I2C1_SDA_PORT, I2C1_SDA_BIT, GPIO_AF_4);
	configurePortPinOpenDrain(I2C1_SDA_PORT, I2C1_SDA_BIT);
//	  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
//	  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
//	  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
//	  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
//	  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

	  LL_I2C_Disable(I2C1);												//Disable I2C1 prior modifying configuration registers

		/* I2C SPEEDCLOCK define to max value: Fast Mode @400kHz */
//		#define I2C_SPEEDCLOCK           400000
//		#define I2C_DUTYCYCLE            LL_I2C_DUTYCYCLE_2

	  LL_I2C_ConfigSpeed(I2C1, 16000000, 400000, LL_I2C_DUTYCYCLE_2);

	  LL_I2C_Enable(I2C1);
}

static inline void configureRadio(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE6);//Connect DIO0 pin to EXTI6 line
	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM6);						//configure EXTI6 for rising trigger interrupt
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM6);								//enable line 6 interrupt in EXTI
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE8);//Connect DIO3 pin to EXTI8 line
	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM8);						//configure EXTI8 for rising trigger interrupt
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM8);								//enable line 8 interrupt in EXTI
	NVIC_SetPriority(EXTI9_5_IRQn, GPIO_PRIORITY);						//enable the EXTI lines interrupt in interrupt controller
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void init_Board_Peripherals(void)
{
	//enable peripherals
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);				//enable port A
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);				//enable port B
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);				//enable port C
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);				//enable port D
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);				//enable system configuration or writing to SYSCFG has no effect
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);				//enable UART1 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);				//enable UART2 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);					//enable SPI2 peripheral
//	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);				//enable low-power timer
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);					//Enable the peripheral clock for I2C1

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);				//trying to enable debug interface

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_DBGMCU, ENABLE);

	//LL_DBGMCU_GetDeviceID(void)
	LL_DBGMCU_EnableDBGStopMode();
	LL_DBGMCU_EnableDBGStandbyMode();
	LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_RTC_STOP);

	//configure inputs
	for(tmp = 0; tmp < sizeof(inputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsInput(inputs[tmp].port, inputs[tmp].bit);

	configurePortPinPullDown(PORT_A, BUTTON_BIT);

	//configure outputs
	boardGreenLedOff();
	boardRedLedOff();
	boardYellowLedOff();
	setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);
	for(tmp = 0; tmp < sizeof(outputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsOutput(outputs[tmp].port, outputs[tmp].bit);

	//configure blue Button
	configureButton();

	//configure GPS interface
	configureUart1();

	//configure command interface
	configureUart2();													//enable and configure UART2

	//configure radio communication interface
	configureSpi2();

    //start RTC, clock source is actually set for the first time only, then proper RTC domain reset is needed to change it
	enableRTC();

	enableLPTim();

	configureI2C1();

//	LL_PWR_EnableWakeUpPin(uint32_t WakeUpPin);

	//radio-related configuration
	configureRadio();

	//activate GPS
	//setPortBit(GPS_ENABLE_PORT, GPS_ENABLE_BIT);
}

/*
//LPTIM is absent on this MCU, using RTC instead. Timeout is 2 - 1024 * 59 i.e. 2/1024s - 59s		(timeout of 1/1024 does not trigger)
void startLpTimer(uint16_t timeout)
{
	volatile uint32_t second;
	volatile uint32_t subsecond;

//	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMB_Disable(RTC);
	RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmB interrupt pending bit

	if(timeout > 60 * 1024 - 1)
		timeout = 60 * 1024 - 1;
	if(timeout < 2)
		timeout = 2;

	//	while(!LL_RTC_IsActiveFlag_RS(RTC));						//shadow regs updated
	second = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC)) + timeout / 1024;
	subsecond = LL_RTC_TIME_GetSubSecond(RTC);
	(void)LL_RTC_ReadReg(RTC, DR);										//unfreeze time/date registers

	timeout %= 1024;
	if(timeout > subsecond) {
		subsecond = 1024 - timeout + subsecond;
		second++;
	} else
		subsecond -= timeout;
	if(second > 59)
		second -= 60;

	while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
	LL_RTC_ALMB_SetSubSecond(RTC, subsecond);
	LL_RTC_ALMB_SetSecond(RTC, __LL_RTC_CONVERT_BIN2BCD(second));
	LL_RTC_ALMB_Enable(RTC);
//	LL_RTC_EnableWriteProtection(RTC);
}
*/

//Timeout range is from 2 to 1024 * 60 * 60 - 1 (3 686 399) i.e. 2/1024s - 60 min (timeout of 1/1024 does not trigger)
//event is the flag to be set when timer expires
void setTimer(uint32_t timeout, uint32_t event)
{
	volatile uint32_t minute;
	volatile uint32_t second;
	volatile uint32_t subsecond;
	TimerQueue newEntry;

//	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMB_Disable(RTC);
//	RTC->ISR = ~(RTC_ISR_ALRBF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmB interrupt pending bit

	//verify timeout range
	if(timeout > 60 * 60 * 1024 - 1)
		timeout = 60 * 60 * 1024 - 1;
	if(timeout < 2)
		timeout = 2;

	//calculate RTC values corresponding to current time + timeout
	//	while(!LL_RTC_IsActiveFlag_RS(RTC));							//shadow regs updated
	minute = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC)) + timeout / (1024 * 60);
	second = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC)) + (timeout % (1024 * 60)) / 1024;
	subsecond = LL_RTC_TIME_GetSubSecond(RTC);
	(void)LL_RTC_ReadReg(RTC, DR);										//unfreeze time/date registers

	timeout %= 1024;
	if(timeout > subsecond) {
		subsecond = 1024 - timeout + subsecond;
		second++;
	} else
		subsecond -= timeout;

	subsecond = 1024 - subsecond;										//this tricky trick is needed for correct time comparison of TimerQueue.time values, should be reverted back when written to RTC alarm subseconds register

	if(second > 59) {
		second -= 60;
		minute++;
	}

	if(minute > 59)
		minute -= 60;

	//add the event to timer schedule
	newEntry.event = event;
	newEntry.minutes = minute;
	newEntry.seconds = second;
	newEntry.subseconds = subsecond;

	for(uint8tmp = 0; uint8tmp < TIMER_SCHEDULE_SIZE; uint8tmp++)		//if the same event is already scheduled remove it first
		if(event == timerSchedule[uint8tmp].event) {
			memmove(&timerSchedule[uint8tmp], &timerSchedule[uint8tmp + 1], (TIMER_SCHEDULE_SIZE - 1 - uint8tmp) * sizeof(TimerQueue));
			timerSchedule[TIMER_SCHEDULE_SIZE - 1].event = HWE_NO_EVENT;//make sure the last entry is free to use now
			break;
		}

	if(HWE_NO_EVENT != timerSchedule[TIMER_SCHEDULE_SIZE - 1].event)	//now check if scheduling table has room for the new event
		ErrorHandler("Timer is full");									//TODO: this is endless loop, should be changed to device reset

	for(uint8tmp = 0; uint8tmp < TIMER_SCHEDULE_SIZE; uint8tmp++) {		//schedule the new event
	//TODO: implement less than 2 subseconds (including 0) time difference between events, by triggering next event simultaneously
	//	if((HWE_NO_EVENT != timerSchedule[uint8tmp].event) && (abs(timerSchedule[uint8tmp].time - newEntry.time) < 10))
	//		break;

		if((HWE_NO_EVENT == timerSchedule[uint8tmp].event) || (newEntry.time <= timerSchedule[uint8tmp].time)) {
			memmove(&timerSchedule[uint8tmp + 1], &timerSchedule[uint8tmp], (TIMER_SCHEDULE_SIZE - 1 - uint8tmp) * sizeof(TimerQueue));
			timerSchedule[uint8tmp].time = newEntry.time;
			timerSchedule[uint8tmp].event = newEntry.event;
			break;
		}
	}

	//reprogram the alarm if the new event timeout expires first
	if(0 == uint8tmp) {
		while(!LL_RTC_IsActiveFlag_ALRBW(RTC));
		LL_RTC_ALMB_SetSubSecond(RTC, 1024 - subsecond);
		LL_RTC_ALMB_SetSecond(RTC, __LL_RTC_CONVERT_BIN2BCD(second));
		LL_RTC_ALMB_SetMinute(RTC, __LL_RTC_CONVERT_BIN2BCD(minute));
	}

	LL_RTC_ALMB_Enable(RTC);
//	LL_RTC_EnableWriteProtection(RTC);
}


void EnableGps(void)
{
	setPortBit(GPS_ENABLE_PORT, GPS_ENABLE_BIT);
}

void DisableGps(void)
{
	resetPortBit(GPS_ENABLE_PORT, GPS_ENABLE_BIT);
}

void startUart2Msg(uint8_t command)
{
	while(HardwareEvents & HWE_UART2_TRANSMITTING);						//make sure not to screw up a message that may be currently in progress
	Uart2Tx.size = 1;													//reserve space for message length
	Uart2Tx.data[Uart2Tx.size++] = command;
}

void addToUart2Msg(uint8_t byte)
{
	if(Uart2Tx.size >= sizeof(Uart2Tx.data))
		ErrorHandler("addToUart2Msg() error");
	else
		Uart2Tx.data[Uart2Tx.size++] = byte;
}

void sendUart2Message(void)
{
	HardwareEvents |= HWE_UART2_TRANSMITTING;							//uart transmission is in progress
	Uart2Tx.data[0] = Uart2Tx.size;										//number of bytes in a message including first length byte
	Uart2Tx.counter = 0;												//TX buffer should be filled and size properly set
	USART_ptr(USART_2)->CR1 |= USART_CR1_TCIE;							//enable transfer complete interrupt initiating interrupt - driven transmission
}

void uart2SendText(char *text)
{
	while(HardwareEvents & HWE_UART2_TRANSMITTING);
	startUart2Msg(RC_TEXT);

	Uart2Tx.size = strlen(text);
	if(Uart2Tx.size > sizeof(Uart2Tx.data) - 2)
		Uart2Tx.size = sizeof(Uart2Tx.data) - 2;
	memcpy(Uart2Tx.data + 2, text, Uart2Tx.size);
	Uart2Tx.size += 2;
	sendUart2Message();
}

void spiAccessRegisters(uint8_t *data, uint8_t length)
{
	resetPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);						//start SPI transaction - assert data frame start

	(void)SPI2->DR;														//read and discard RX buffer to clear RXNE, just in case

	while(length--)
	{
		SPI2->DR = *data;												//write a data byte
		while(0 == (SPI2->SR & SPI_SR_RXNE));							//wait until reply is ready
		*(data++) = SPI2->DR;											//save the reply. * Address byte will be overwritten with 0
	}

	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//end SPI access - deactivate NSS line
}

void spiAccessRegisters2(uint8_t command, uint8_t *data, uint8_t length)
{
	resetPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);						//assert data frame start

	(void)SPI2->DR;														//read and discard RX buffer to clear RXNE, just in case

	SPI2->DR = command;													//send the address with the Write bit indicator
	while(0 == (SPI2->SR & SPI_SR_RXNE));								//wait until address transmits
	(void)SPI2->DR;														//discard dummy return, otherwise next byte returned would be lost

	while(length--)
	{
		SPI2->DR = *data;												//write a data byte
		while(0 == (SPI2->SR & SPI_SR_RXNE));							//wait until reply is ready
		*(data++) = SPI2->DR;											//save the reply
	}

	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//deactivate NSS line
}


//the stop parameter should be zero if i2cRead is called right after i2cWrite, to generate restart condition
void i2cWrite(uint8_t address, uint8_t *data, uint8_t length, uint8_t stop)
{
	LL_I2C_GenerateStartCondition(I2C1);								//Initiate a Start condition to the Slave device

	while(!LL_I2C_IsActiveFlag_SB(I2C1));								//wait until Start Bit transmitted (SB flag raised)

	LL_I2C_TransmitData8(I2C1, (address << 1));							//Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a write request

	while(!LL_I2C_IsActiveFlag_ADDR(I2C1));								//wait until Address Acknowledgement received (ADDR flag raised)

	LL_I2C_ClearFlag_ADDR(I2C1);										//Clear ADDR flag value in ISR register

	while(length > 0)													//Transmit data
		if(LL_I2C_IsActiveFlag_TXE(I2C1)) {								//Check TXE flag value in ISR register and write next data before previous is transmitted
			LL_I2C_TransmitData8(I2C1, (*data++));						//Write data in Transmit Data register. TXE flag is cleared by writing data in TXDR register
			length--;
		}

	if(stop) {
		while(!LL_I2C_IsActiveFlag_TXE(I2C1));
		LL_I2C_GenerateStopCondition(I2C1);								//Generate Stop condition
	}

}

void i2cRead(uint8_t address, uint8_t *data, uint8_t length)
{
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);						//Prepare acknowledge for Master data reception

	LL_I2C_GenerateStartCondition(I2C1);								//Initiate a Start condition to the Slave device

	while(!LL_I2C_IsActiveFlag_SB(I2C1));								//wait until Start Bit transmitted (SB flag raised)

	LL_I2C_TransmitData8(I2C1, (address << 1) | I2C_REQUEST_READ);		//Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a write request

	while(!LL_I2C_IsActiveFlag_ADDR(I2C1));								//wait until Address Acknowledgement received (ADDR flag raised)

	if(1 == length)														//for one-byte reception NAK is activated here
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);

	LL_I2C_ClearFlag_ADDR(I2C1);										//Clear ADDR flag value in ISR register

	//here master is in receive mode activated by I2C_REQUEST_READ bit
	while(length > 1)													//receive data
		if(LL_I2C_IsActiveFlag_RXNE(I2C1)) {							//Check RXNE flag value in ISR register and read next data byte
			(*data++) = LL_I2C_ReceiveData8(I2C1);						//Read data in Receive Data register. RXNE flag is cleared by reading data from RXDR register
			length--;
		}


	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);						//generate NACK
	LL_I2C_GenerateStopCondition(I2C1);									//and Stop condition after receiving next byte

	while(!LL_I2C_IsActiveFlag_RXNE(I2C1));								//get the last data byte
	*data = LL_I2C_ReceiveData8(I2C1);
}


inline void boardGreenLedOn(void)
{
	resetPortBit(AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT);
}

inline void boardGreenLedOff(void)
{
	setPortBit(AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT);
}

void boardGreenLedToggle(void)
{
	if(readPortBit(AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT))
		boardGreenLedOn();
	else
		boardGreenLedOff();
}

inline void boardYellowLedOn(void)
{
	resetPortBit(AUX_YELLOW_LED_PORT, AUX_YELLOW_LED_BIT);
}

inline void boardYellowLedOff(void)
{
	setPortBit(AUX_YELLOW_LED_PORT, AUX_YELLOW_LED_BIT);
}

void boardYellowLedToggle(void)
{
	if(readPortBit(AUX_YELLOW_LED_PORT, AUX_YELLOW_LED_BIT))
		boardYellowLedOn();
	else
		boardYellowLedOff();
}

inline void boardRedLedOn(void)
{
	resetPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT);
}

inline void boardRedLedOff(void)
{
	setPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT);
}

void boardRedLedToggle(void)
{
	if(readPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT))
		boardRedLedOn();
	else
		boardRedLedOff();
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
	if(switches & 0b100)
		setPortBit(RF_SWITCH_CONTROL_1_PORT, RF_SWITCH_CONTROL_1_BIT);
	else
		resetPortBit(RF_SWITCH_CONTROL_1_PORT, RF_SWITCH_CONTROL_1_BIT);

	if(switches & 0b010)
		setPortBit(RF_SWITCH_CONTROL_2_PORT, RF_SWITCH_CONTROL_2_BIT);
	else
		resetPortBit(RF_SWITCH_CONTROL_2_PORT, RF_SWITCH_CONTROL_2_BIT);

	if(switches & 0b001)
		setPortBit(RF_PA_HIGH_PWR_PORT, RF_PA_HIGH_PWR_BIT);
	else
		resetPortBit(RF_PA_HIGH_PWR_PORT, RF_PA_HIGH_PWR_BIT);
}


void enterSleep(void)
{
	LL_PWR_ClearFlag_WU();												//ensure that the WUF  wake up flag is cleared
	LL_LPM_EnableDeepSleep();											//Set SLEEPDEEP bit of Cortex System Control Register
	__WFI();															//Request Wait For Interrupt

	//Request Wait For Event
/*	__SEV();
	__WFE();
	__WFE();
*/
	//TODO: is it necessary?
//	CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);							//Reset SLEEPDEEP bit after exiting from Stop

	LL_RCC_HSI_Enable();												//Enable HSI
	while(0 == LL_RCC_HSI_IsReady());									//wait for HSI getting stable
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);					//set HSI as system clock
}


void enterStandby(void)
{
	uart2SendText("Bye, going to StandBy");
	while(HardwareEvents & HWE_UART2_TRANSMITTING);						//wait until the message transmits

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
	uart2SendText(msg);
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
