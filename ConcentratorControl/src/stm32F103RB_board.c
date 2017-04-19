//MCU programming primitives and board-specific hardware functions

#include <stm32F103RB_board.h>
#include <string.h>


#define VECT_TAB_OFFSET			0x00								//Vector Table base offset field. This value must be a multiple of 0x200
#define RCC_PLLSOURCE_HSI_DIV2	((uint32_t)0x00000000)				//HSI clock divided by 2 selected as PLL entry clock source


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

#define CONFIG_MODE_MASK		0b1111								//used to clear mode and configuration bits for port pins

typedef enum
{
	GPIO_INPUT					= 0b00,
	GPIO_OUTPUT_10MHz			= 0b01,
	GPIO_OUTPUT_02MHz			= 0b10,
	GPIO_OUTPUT_50MHz			= 0b11,
} GPIO_Pin_Mode;

typedef enum
{
	GPIO_INPUT_ANALOG			= 0b00 << 2,						//Analog mode
	GPIO_INPUT_FLOATING			= 0b01 << 2,						//Floating input (reset state)
	AF_INPUT_PULL				= 0b10 << 2,						//Input with pull-up / pull-down
	AF_INPUT_RESERVED			= 0b11 << 2,						//Reserved
	GPIO_OUTPUT_PUSH_PULL		= 0b00 << 2,						//General purpose output push-pull
	GPIO_OUTPUT_OPEN_DRAIN		= 0b01 << 2,						//General purpose output Open-drain
	AF_OUTPUT_PUSH_PULL			= 0b10 << 2,						//Alternate function output Push-pull
	AF_OUTPUT_OPEN_DRAIN		= 0b11 << 2,						//Alternate function output Open-drain
} GPIO_Pin_Type;

typedef enum
{
	ALARM_PRIORITY				= 0,
	TIMER_3_PRIORITY,
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

static const struct GPIO_Line inputs[] = {{BUTTON_PORT, BUTTON_BIT},
		{RADIO_DIO0_PORT, RADIO_DIO0_BIT}, {RADIO_DIO1_PORT, RADIO_DIO1_BIT}, {RADIO_DIO2_PORT, RADIO_DIO2_BIT},
		{RADIO_DIO3_PORT, RADIO_DIO3_BIT}, {RADIO_DIO4_PORT, RADIO_DIO4_BIT}, {RADIO_DIO5_PORT, RADIO_DIO5_BIT}};

static const struct GPIO_Line outputs[] = {/*{LED_PORT, LED_BIT},*/ {AUX_RED_LED_PORT, AUX_RED_LED_BIT}, {AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT},
		{RADIO_RESET_PORT, RADIO_RESET_BIT}, {RADIO_RXTX_EXT_PORT, RADIO_RXTX_EXT_BIT}, {RADIO_NSS_PORT, RADIO_NSS_BIT}};


void SystemInit (void)
{
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);						//Enable power interface clock
	SET_BIT(RCC->CR, RCC_CR_HSION);									//Enable HSI
	while(0 == READ_BIT(RCC->CR, RCC_CR_HSIRDY));					//wait for HSI getting stable
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);								//Disable the main PLL to configure it's parameters
	while(READ_BIT(RCC->CR, RCC_CR_PLLRDY));						//Wait till PLL is disabled
    //Configure HSI/2 as the main PLL clock source and multiplication factor of 12 to get 48Mhz system clock
    MODIFY_REG(RCC->CFGR, (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL),((RCC_PLLSOURCE_HSI_DIV2) | (RCC_CFGR_PLLMULL12) ));
    //MODIFY_REG(RCC->CFGR, (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL),((RCC_PLLSOURCE_HSI_DIV2) | (RCC_CFGR_PLLMULL9) ));
    SET_BIT(RCC->CR, RCC_CR_PLLON);									//Enable the main PLL
    while(0 == READ_BIT(RCC->CR, RCC_CR_PLLRDY));					//Wait till PLL is ready
    FLASH->ACR = (FLASH->ACR & (~FLASH_ACR_LATENCY)) | FLASH_ACR_LATENCY_1;	//match flash access speed with that of CPU
	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);		//HCLK (AHB) Configuration: SYSCLK not divided i.e. 48Mhz
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);			//set PLL as system clock
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);		//clock APB1 with 24Mhz
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE1_DIV2 << 3);//clock APB2 with 24Mhz

//	RCC->CIER = 0x00000000;											//Disable all clock interrupts
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; 						//Vector Table location add offset address in Internal FLASH
}

inline void setPortBit(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->BSRR = (0x01 << pin);
}


inline void resetPortBit(GPIO_Port base, GPIO_Number pin)
{
	GPIO_ptr(base)->BRR = (0x01 << pin);
}


inline uint32_t readPortBit(GPIO_Port base, GPIO_Number pin)
{
	return (GPIO_ptr(base)->IDR & (0x01 << pin));
}


//base is port A, B or C base address, pin is port bit like PA/PB/PC 0 - PA/PB/PC 15
void configureGpioPin(GPIO_Port base, GPIO_Number pin, GPIO_Pin_Type type, GPIO_Pin_Mode mode)
{
	if(pin < GPIO_8) {															//High or Low Control Register
		GPIO_ptr(base)->CRL &= ~(CONFIG_MODE_MASK << (pin * 4));				//clear configuration and mode bits
		GPIO_ptr(base)->CRL |= ((type | mode) << (pin * 4));					//set as floating point input
	} else {
		GPIO_ptr(base)->CRH &= ~(CONFIG_MODE_MASK << ((pin - GPIO_8) * 4));		//clear configuration and mode bits
		GPIO_ptr(base)->CRH |= ((type | mode) << ((pin - GPIO_8) * 4));			//set as floating point input
	}
}


static inline void configureButton(void)
{
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;							//Select Port C for EXTI13 line input source
	EXTI->IMR |= EXTI_IMR_IM13;											//enable EXTI13 line interrupt
	EXTI->RTSR |= EXTI_RTSR_RT13;										//Rising trigger event configuration bit of line 13
	EXTI->FTSR |= EXTI_FTSR_FT13;										//Falling trigger event configuration bit of line 13
	NVIC_SetPriority(EXTI15_10_IRQn, GPIO_PRIORITY);					//enable button interrupts in interrupt controller
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void configureUart(USARTx base)
{
	USART_ptr(base)->CR1 &=  ~USART_CR1_UE;								//disable UART in order to program
	USART_ptr(base)->BRR = 0xD0;										//115200 baud from 24MHz PCLK1: 16*USARTDIV = 208.(3), USARTDIV = 13.02, BRR = 0xD0
	USART_ptr(base)->CR1 |= USART_CR1_RXNEIE;							//Enable interrupt on character reception
	USART_ptr(base)->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;	//enable UART, UART receiver and UART transmitter
}


static void inline configureUart2(void)
{
	configureGpioPin(UART2_TX_PORT, UART2_TX_BIT, AF_OUTPUT_PUSH_PULL, GPIO_OUTPUT_10MHz);	//configure UART TX output
	configureGpioPin(UART2_RX_PORT, UART2_RX_BIT, AF_INPUT_PULL, GPIO_INPUT);				//configure UART RX input
	configureUart(USART_2);												//configure UART communication parameters
	NVIC_SetPriority(USART2_IRQn, USART_2_PRIORITY);					//enable UART2 interrupt in interrupt controller
	NVIC_EnableIRQ(USART2_IRQn);
}


static void inline configureSpi1(void)
{
	configureGpioPin(RADIO_MOSI_PORT, RADIO_MOSI_BIT, AF_OUTPUT_PUSH_PULL, GPIO_OUTPUT_10MHz);
	configureGpioPin(RADIO_MISO_PORT, RADIO_MISO_BIT, AF_INPUT_PULL, GPIO_INPUT);
	configureGpioPin(RADIO_SCK_PORT, RADIO_SCK_BIT, AF_OUTPUT_PUSH_PULL, GPIO_OUTPUT_10MHz);
	setPortBit(RADIO_NSS_PORT, RADIO_NSS_BIT);							//deactivate NSS line

	SPI1_ptr->CR1 &= ~SPI_CR1_SPE;										//disable SPI1 in order to program
	SPI1_ptr->CR1 |= QUARTER_PERIPHERAL_CLOCK << SPI_CR1_BR_Pos			//6MHz from 24Hz peripheral clock
			 | SPI_CR1_MSTR												//SPI1 will be master
			 | SPI_CR1_SSM												//Software slave management
			 | SPI_CR1_SSI;												//internal slave select to avoid MODF error
	SPI1_ptr->CR1 |= SPI_CR1_SPE;										//enable SPI1
}


void enableRtcConfig(void)												//sets Configuration bit to write into Prescaler and Counter registers
{
	while(!(RTC_ptr->CRL & RTC_CRL_RTOFF));								//wait for previous write operation to RTC registers to complete
	SET_BIT(RTC_ptr->CRL, RTC_CRL_CNF);									//Disable the write protection for RTC registers
}


void disableRtcConfig(void)												//resets Configuration bit to enable into Prescaler and Counter registers write protection
{
	CLEAR_BIT(RTC_ptr->CRL, RTC_CRL_CNF);								//Disable the write protection for RTC registers
	while(!(RTC_ptr->CRL & RTC_CRL_RTOFF));								//wait for previous write operation to RTC registers to complete
}


static inline void enableRTC(void)
{
	//Enable access to RTC and backup registers
	*(__IO uint32_t *) ((uint32_t)(PERIPH_BB_BASE + (((PWR_BASE - PERIPH_BASE) + 0x00) * 32) + (POSITION_VAL(PWR_CR_DBP) * 4))) = (uint32_t)ENABLE;
	SET_BIT(PWR->CR, PWR_CR_DBP);										//Enable write access to Backup domain, is it same as line above?
    while(!(PWR->CR & PWR_CR_DBP));										//Wait for Backup domain Write protection disable
    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);									//turn LSE On

#define CR_REG_INDEX                     ((uint8_t)1)
#define BDCR_REG_INDEX                   ((uint8_t)2)
#define RCC_FLAG_MASK                    ((uint8_t)0x1F)
#define RCC_FLAG_LSERDY	((uint8_t)((BDCR_REG_INDEX << 5) | POSITION_VAL(RCC_BDCR_LSERDY)))
#define __HAL_RCC_GET_FLAG(__FLAG__) (((((__FLAG__) >> 5) == CR_REG_INDEX)?   RCC->CR   : \
                                      ((((__FLAG__) >> 5) == BDCR_REG_INDEX)? RCC->BDCR : \
                                                                              RCC->CSR)) & ((uint32_t)1 << ((__FLAG__) & RCC_FLAG_MASK)))
    while(!__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY));						//wait for LSE to become ready

    //Reset the Backup domain to change RTC Clock source if it's not LSE already
    if(RCC_BDCR_RTCSEL_LSE != (RCC->BDCR & RCC_BDCR_RTCSEL))
    {
    	uint32tmp = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));
    	(*(__IO uint32_t *) ((uint32_t)(PERIPH_BB_BASE + (((RCC_BASE - PERIPH_BASE) + 0x20) * 32) + (POSITION_VAL(RCC_BDCR_BDRST) * 4))) = ENABLE);
    	(*(__IO uint32_t *) ((uint32_t)(PERIPH_BB_BASE + (((RCC_BASE - PERIPH_BASE) + 0x20) * 32) + (POSITION_VAL(RCC_BDCR_BDRST) * 4))) = DISABLE);
    	RCC->BDCR = uint32tmp | RCC_BDCR_RTCSEL_LSE;					//Restore the Content of BDCR register and set LSE as RTC clock
      //enable RTC
      (*(__IO uint32_t *) ((uint32_t)(PERIPH_BB_BASE + (((RCC_BASE - PERIPH_BASE) +  0x20) * 32) + (POSITION_VAL(RCC_BDCR_RTCEN) * 4))) = ENABLE);
    }

    enableRtcConfig();
    RTC_ptr->PRLL = 31;													//this provides 1/1024 of a second clock ~1ms
    disableRtcConfig();

    //may need to connect tamper pin to RTC
    RTC_ptr->CRL = ~RTC_CRL_ALRF;										//clear alarm flag
    SET_BIT(RTC_ptr->CRH, RTC_CRH_ALRIE);								//Enable RTC alarm
    SET_BIT(EXTI->IMR, ((uint32_t)EXTI_IMR_MR17));						//enable external interrupt for line 17 Connected to the RTC Alarm event
    SET_BIT(EXTI->RTSR, ((uint32_t)EXTI_IMR_MR17));						//configure the line to trigger on rising edge

	NVIC_SetPriority(RTC_Alarm_IRQn, ALARM_PRIORITY);					//enable RTC Alarms interrupts in interrupt controller
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
}


static inline void configureTimer3(void)
{
	//TIM3_ptr->CR1 = TIM_CR1_OPM | TIM_CR1_URS;							//The counter counts strictly up, One-pulse mode, Only counter overflow generates interrupt
	TIM3_ptr->CR1 =  TIM_CR1_URS;										//The counter counts strictly up
	TIM3_ptr->PSC = 48000 - 1;											//Set the Prescaler to output 1ms clock out of 48MHz
	TIM3_ptr->EGR = TIM_EGR_UG;											//Force Prescaler value to take effect immediately
	TIM3_ptr->DIER |= TIM_DIER_UIE;										//enable update interrupt
	NVIC_SetPriority(TIM3_IRQn, TIMER_3_PRIORITY);
	NVIC_EnableIRQ(TIM3_IRQn);
}


static inline void configureRadio(void)
{
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PA;							//Connect DIO0 to EXTI10 line
	EXTI->IMR |= EXTI_IMR_IM10;											//enable EXTI10 line interrupt
	EXTI->RTSR |= EXTI_RTSR_RT10;										//Rising edge trigger
	NVIC_SetPriority(EXTI15_10_IRQn, GPIO_PRIORITY);					//enable interrupt in interrupt controller
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	//turn the radio on, TODO: it's ready only in 10ms after this
	setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);
}


void init_Board_Peripherals(void)
{
//while(1);

	//enable peripherals
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);							//enable port A
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);							//enable port B
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);							//enable port C
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);						//enable UART2 peripheral
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);							//enable SPI1 peripheral
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);							//enable timer 3
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);							//enable RCC peripheral clock to change the source clock of the RTC
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);							//enable RCC peripheral clock to change the source clock of the RTC
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);							//alternate function IO enable

	//configure inputs
	for(tmp = 0; tmp < sizeof(inputs) / sizeof(struct GPIO_Line); tmp++)
		configureGpioPin(inputs[tmp].port, inputs[tmp].bit, GPIO_INPUT_FLOATING, GPIO_INPUT);

	//configure outputs
	for(tmp = 0; tmp < sizeof(outputs) / sizeof(struct GPIO_Line); tmp++)
		configureGpioPin(outputs[tmp].port, outputs[tmp].bit, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_10MHz);

	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;									//to use PA15 as GPIO, not JTAG

	//configure blue Button
	configureButton();

	//configure command interface
	configureUart2();													//enable and configure UART2

	//configure radio communication interface
	configureSpi1();

	configureTimer3();

    //start RTC, clock source is actually set for the first time only, then proper RTC domain reset is needed to change it
	enableRTC();

	//configure radio
	configureRadio();
}

//starts one-shot timer to timeout milliseconds
//void startLpTimer(uint16_t timeout)
void startTimer3(uint16_t timeout)
{
	TIM3_ptr->ARR = timeout;											//Set the Auto reload value
	TIM3_ptr->EGR = TIM_EGR_UG;											//Re-initialize the counter and update the registers
	TIM3_ptr->CR1 |= TIM_CR1_CEN;										//start the timer
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
//	while(HardwareEvents & HWE_UART2_TRANSMITTING);
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

	(void)SPI1_ptr->DR;													//read and discard RX buffer to clear RXNE, just in case
	while(length--) {
		SPI1_ptr->DR = *data;											//write a data byte
		while(0 == (SPI1_ptr->SR & SPI_SR_RXNE));						//wait until reply is ready
		*(data++) = SPI1_ptr->DR;										//save the reply. * Address byte will be overwritten with 0
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
		resetPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT);
	else
		setPortBit(AUX_RED_LED_PORT, AUX_RED_LED_BIT);
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


void driveRfSwitch(uint8_t high_low)
{
	if(high_low)
		setPortBit(RADIO_RXTX_EXT_PORT, RADIO_RXTX_EXT_BIT);
	else
		resetPortBit(RADIO_RXTX_EXT_PORT, RADIO_RXTX_EXT_BIT);
}

//fq is actual frequency in 61.035Hz steps in reversed order
void RadioSetCommunicationParameters(uint32_t fq, RadioBW1276 bw, RadioSF sf, RadioCR cr, RadioEH eh)
{
	uint32tmp = (0x00 << 8) | RC_SET_MODEM_MODE;					//put device to sleep to set lora mode
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x80 << 8) | RC_SET_MODEM_MODE;					//set LoRa mode bit
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;					//move to standby mode for further programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

//	uint32tmp = (0x00 << 24) | (0xC0 << 16) | (0xE4 << 8) | RC_SET_FREQUENCY;				//set 915MHz
//	fq /= 61.03515625;											//set desired frequency 61Hz steps TODO: replace with constant
//	uint32tmp = ((fq & 0xFF) << 24) | ((fq >> 8 & 0xFF) << 16) | ((fq >> 16 & 0xFF) << 8) | RC_SET_FREQUENCY;
	uint32tmp = (fq << 8) | RC_SET_FREQUENCY;
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

//	uint32tmp = (0x8F << 8) | RC_SET_PA_CONFIG;						//output from PA not RFO pin, use max power
//	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	//uint32tmp = (0xC0 << 16) | (0x72 << 8) | RC_SET_MODEM_CONFIG12_;			//set SF12, CR 4/5, CRC, Explicit header
	uint32tmp = ((sf << 4) | 0x04) << 16;							//SF and RxPayloadCrcOn is ON
	uint32tmp |= ((bw << 4) | (cr << 1) | eh) << 8;					//BW, CR, and  EH
	uint32tmp |= RC_SET_MODEM_CONFIG12_;
	spiAccessRegisters((uint8_t *)&uint32tmp, 3);

//	uint32tmp  = (0x0B? << 8) | RC_SET_MODEM_CONFIG3;
	uint32tmp  = (0x0C << 8) | RC_SET_MODEM_CONFIG3;				//LowDataRateOptimize is ON and LNA gain set by the internal AGC loop
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp  = (0x03 << 8) | RC_SET_DETECTION_OPTIMIZE;			//TODO: calculate Detection Optimize depending on BW and SF)
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
}

//RadioSetCommunicationParameters() should be called before to set FIFO pointers and RX parameters
void RadioTransmit(uint8_t *data, uint8_t length)
{
	driveRfSwitch(0x01);											//set to TX from PA

	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;					//move to standby mode for programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);


	uint32tmp = RC_SET_FIFO_POINTER;								//set FIFO pointer, TX and RX base addresses to 00
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	spiAccessRegisters2(RC_SET_FIFO, data, length);					//fill FIFO with the data to transmit
	uint32tmp = (length << 8) | RC_SET_PAYLOAD_LENGTH;				//set message length
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x40 << 8) | RC_SET_DIO_MAPPINGS;					//configure DIO0 for TX done interrupt TODO: touch only DIO0
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

//	uint32tmp = (0xFF << 8) | RC_SET_IRQ_FLAGS;						//reset all IRQ flags
//	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x83 << 8) | RC_SET_MODEM_MODE;					//start transmission
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	HardwareEvents |= HWE_RADIO_TRANSMITTING;
}


//void RadioReceive(uint32_t fq, RadioBW1272 bw, RadioSF sf, RadioCR cr, RadioEH eh)
//RadioSetCommunicationParameters() should be called before to set FIFO pointers and TX parameters
void RadioReceive(void)
{
	driveRfSwitch(0x00);											//set to RX
//	uint32tmp = (0x00 << 16) | (0x00 << 8) | RC_SET_FIFO_POINTER;	//set FIFO and TX base addresses to 00
//	spiAccessRegisters((uint8_t *)&uint32tmp, 3);

	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;					//move to standby mode for programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x00 << 8) | RC_SET_DIO_MAPPINGS;					//configure DIO0 for RX done interrupt TODO: touch only DIO0
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (0x85 << 8) | RC_SET_MODEM_MODE;					//start continuous reception TODO: use mode constants
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	HardwareEvents |= HWE_RADIO_RECEIVING;
}

//This function is executed in case of error occurrence.
void ErrorHandler(char *msg)
{
	boardRedLedOn();
	uart2SendText(msg);
	while(1);
}

#ifdef  USE_FULL_ASSERT
//Reports the name of the source file and the source line number where the assert_param error has occurred.
void assert_failed(uint8_t* file, uint32_t line)
{
//User can add his own implementation to report the file name and line number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	while (1);
}
#endif
