//MCU programming primitives and board-specific hardware functions

#include "stm32l05x_board.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include "radio.h"
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

static const struct GPIO_Line inputs[] = {{BUTTON_PORT, BUTTON_BIT},
		{RADIO_DIO0_PORT, RADIO_DIO0_BIT}, {RADIO_DIO1_PORT, RADIO_DIO1_BIT}, {RADIO_DIO2_PORT, RADIO_DIO2_BIT},
		{RADIO_DIO3_PORT, RADIO_DIO3_BIT}, {RADIO_DIO4_PORT, RADIO_DIO4_BIT}, {RADIO_DIO5_PORT, RADIO_DIO5_BIT}};

static const struct GPIO_Line outputs[] = {/*{LED_PORT, LED_BIT},*/ {AUX_RED_LED_PORT, AUX_RED_LED_BIT}, {AUX_GREEN_LED_PORT, AUX_GREEN_LED_BIT},
		{RADIO_RESET_PORT, RADIO_RESET_BIT}, {RADIO_RXTX_EXT_PORT, RADIO_RXTX_EXT_BIT}, {RADIO_NSS_PORT, RADIO_NSS_BIT},
		{RADIO_MOSI_PORT, RADIO_MOSI_BIT}, {RADIO_SCK_PORT, RADIO_SCK_BIT}};


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


static inline void configureButton(void)
{
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;						//Select Port C for EXTI13 line input source
	EXTI->IMR |= EXTI_IMR_IM13;											//enable EXTI13 line interrupt
	EXTI->RTSR |= EXTI_RTSR_RT13;										//Rising trigger event configuration bit of line 13
	EXTI->FTSR |= EXTI_FTSR_FT13;										//Falling trigger event configuration bit of line 13
	NVIC_SetPriority(EXTI4_15_IRQn, GPIO_PRIORITY);						//enable button interrupts in interrupt controller
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}


void configureUart(USARTx base)
{
	USART_ptr(base)->CR1 &=  ~USART_CR1_UE;								//disable UART in order to program
	USART_ptr(base)->BRR = 0x8A;										//115200 baud from 16MHz HSI

//	USARTx_CR1(base) |= USART_CR1_RTOIE;								//enable interrupt on receiver time-out
//	USARTx_CR2(base) |= USART_CR2_RTOEN;								//enable receiver time-out function
//	USARTx_RTOR(base) |= 0xFFFF;										//receiver time-out value

	USART_ptr(base)->CR1 |= USART_CR1_UESM | USART_CR1_RXNEIE;			//Enable UART in STOP Mode and interrupt on character reception, also used for wake up from Stop Mode
//	USARTx_CR3(base) |= USART_CR3_WUFIE;								//Wake Up Interrupt Enable
//	USARTx_CR3(base) |= 0b10 << USART_CR3_WUS_Pos;						//WuF active on Start bit detection, This bit field can only be written when the USART is disabled (UE=0)
	USART_ptr(base)->CR3 |= USART_CR3_UCESM;							//Clock Enable in Stop mode *feature removed in 2015

	USART_ptr(base)->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;	//enable UART, UART receiver and UART transmitter
}

static void inline configureUart2(void)
{
	configurePortPinAsAf(UART2_TX_PORT, UART2_TX_BIT, GPIO_AF_4);		//configure UART TX GPIO
	configurePortPinAsAf(UART2_RX_PORT, UART2_RX_BIT, GPIO_AF_4);		//configure UART RX GPIO
	LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_HSI);			//set HSI as UART2 clock
	configureUart(USART_2);												//configure UART communication parameters
	NVIC_SetPriority(USART2_IRQn, USART_2_PRIORITY);					//enable UART2 interrupt in interrupt controller
	NVIC_EnableIRQ(USART2_IRQn);
}

static inline void configureSpi1(void)
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
	//TODO: make sure RTC domain reset is not required to set RTC clock source for the first time
	LL_PWR_EnableBkUpAccess();											//Enable access to the backup domain
	while(!LL_PWR_IsEnabledBkUpAccess());
	LL_RCC_LSE_Enable();												//Enable LSE
	while(!LL_RCC_LSE_IsReady());										//wait for LSE to become ready
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);					//set LSE as RTC clock
	LL_RCC_EnableRTC();													//at last, enable RTC

	//RTC alarm A configuration
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_ALMA_Disable(RTC);
	RTC->ISR = ~(RTC_ISR_ALRAF | RTC_ISR_INIT) | (RTC->ISR & RTC_ISR_INIT);	//Clear the AlarmA interrupt pending bit
	while(!LL_RTC_IsActiveFlag_ALRAW(RTC));

//	LL_RTC_ALMA_SetMask(RTC, RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1);	//trigger every second
	LL_RTC_EnableIT_ALRA(RTC);											//enable Alarm A interrupt
//	LL_RTC_ALMA_Enable(RTC);											//enable Alarm A

	LL_RTC_EnableWriteProtection(RTC);

	LL_EXTI_EnableRisingTrig_0_31(EXTI_IMR_IM17);						//connect RTC Alarm to EXTI line
	LL_EXTI_EnableIT_0_31(EXTI_IMR_IM17);

	NVIC_SetPriority(RTC_IRQn, ALARM_PRIORITY);							//enable RTC Alarms interrupts in interrupt controller
	NVIC_EnableIRQ(RTC_IRQn);
}


static inline void enableLPTim(void)
{
	LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSE);
	LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV32);			//~1ms clock
	LL_LPTIM_EnableTimeout(LPTIM1);										//enable wake-up
	LL_LPTIM_EnableIT_CMPM(LPTIM1);										//Enable Compare match interrupt
	LL_LPTIM_Enable(LPTIM1);											//Enable LPTIM

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_29);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_29);						//connect RTC Alarm to EXTI line

	NVIC_SetPriority(LPTIM1_IRQn, LPTIMER_PRIORITY);
	NVIC_EnableIRQ(LPTIM1_IRQn);

	LL_LPTIM_SetAutoReload(LPTIM1, 0xFFFF);								//Load the period value has to be strictly bigger than compare value
//	LL_LPTIM_SetCompare(LPTIM1, 100);									//Load the Timeout value
//	LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS);	//LL_LPTIM_OPERATING_MODE_ONESHOT
}

static inline void configureRadio(void)
{
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PA;						//Connect DIO0 to EXTI10 line
	EXTI->IMR |= EXTI_IMR_IM10;											//enable EXTI10 line interrupt
	EXTI->RTSR |= EXTI_RTSR_RT10;										//Rising edge trigger
	NVIC_SetPriority(EXTI4_15_IRQn, GPIO_PRIORITY);					//enable interrupt in interrupt controller
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	//turn the radio on, TODO: it's ready only in 10ms after this
	setPortBit(RADIO_RESET_PORT, RADIO_RESET_BIT);
}


void init_Board_Peripherals(void)
{
	//enable peripherals
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);					//enable port A
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);					//enable port B
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);					//enable port C
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);				//enable system configuration or writing to SYSCFG has no effect
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);				//enable UART2 peripheral
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);					//enable SPI1 peripheral
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);				//enable low-power timer

	//configure inputs
	for(tmp = 0; tmp < sizeof(inputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsInput(inputs[tmp].port, inputs[tmp].bit);

	//configure outputs
	for(tmp = 0; tmp < sizeof(outputs) / sizeof(struct GPIO_Line); tmp++)
		configurePortPinAsOutput(outputs[tmp].port, outputs[tmp].bit);

	//configure blue Button
	configureButton();

	//configure command interface
	configureUart2();													//enable and configure UART2

	//configure radio communication interface
	configureSpi1();

    //start RTC, clock source is actually set for the first time only, then proper RTC domain reset is needed to change it
	enableRTC();

	enableLPTim();

	//configure radio
	configureRadio();
}

//starts one-shot timer to timeout milliseconds
void startLpTimer(uint16_t timeout)
{
	LL_LPTIM_Enable(LPTIM1);
	LL_LPTIM_SetCompare(LPTIM1, timeout);								//Load the Timeout value
	LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT);		//LL_LPTIM_OPERATING_MODE_CONTINUOUS
}

void stopLpTimer(void)
{
	LL_LPTIM_Disable(LPTIM1);
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


void driveRfSwitch(uint8_t high_low)
{
	if(high_low)
		setPortBit(RADIO_RXTX_EXT_PORT, RADIO_RXTX_EXT_BIT);
	else
		resetPortBit(RADIO_RXTX_EXT_PORT, RADIO_RXTX_EXT_BIT);
}
//fq is actual frequency in 61.035Hz steps in reversed order
void RadioSetCommunicationParameters(RadioSettings *rs)
{
	uint32tmp = (0x00 << 8) | RC_SET_MODEM_MODE;						//put device to sleep to set lora mode
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x80 << 8) | RC_SET_MODEM_MODE;						//set LoRa mode bit
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);
	uint32tmp = (0x81 << 8) | RC_SET_MODEM_MODE;						//move to standby mode for further programming
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp = (rs->fq << 8) | RC_SET_FREQUENCY;						//frequency in 61.035Hz steps in reversed order
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

//	uint32tmp = (0x8F << 8) | RC_SET_PA_CONFIG;							//output from PA not RFO pin, use max power
//	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

	uint32tmp = RC_SET_FIFO_POINTER;									//set FIFO pointer, TX and RX base addresses to 00
	spiAccessRegisters((uint8_t *)&uint32tmp, 4);

//	uint32tmp = ((rs->sf << 4) | 0x04) << 16;							//SF and  LNA gain set by the internal AGC loop
//	uint32tmp |= ((rs->bw << 6) | (rs->cr << 3) | (rs->eh << 2) | 0x03) << 8;	//BW, SF, CR, EH as well as hardcoded payload CRC and LowDataRateOptimize enabled

	uint32tmp = ((rs->sf << 4) | 0x04) << 16;							//SF and RxPayloadCrcOn is ON
	uint32tmp |= ((rs->bw << 4) | (rs->cr << 1) | rs->eh) << 8;			//BW, CR, and  EH
	uint32tmp |= RC_SET_MODEM_CONFIG12_;
	spiAccessRegisters((uint8_t *)&uint32tmp, 3);

	uint32tmp  = (0x0C << 8) | RC_SET_MODEM_CONFIG3;					//LowDataRateOptimize is ON and LNA gain set by the internal AGC loop
	spiAccessRegisters((uint8_t *)&uint32tmp, 2);

	uint32tmp  = (0x03 << 8) | RC_SET_DETECTION_OPTIMIZE;				//TODO: calculate Detection Optimize depending on BW and SF)
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


void enterSleep(void)
{
	//LL_PWR_ClearFlag_WU();
	SET_BIT(PWR->CR, PWR_CR_CWUF);										//ensure that the WUF  wake up flag is cleared
//	USARTx_CR3(USART_2) |= USART_CR3_WUFIE;								//Wake Up Interrupt Enable
//	USARTx_CR1(USART_2) |= USART_CR1_UESM;								//Enable USART in STOP Mode
	//LL_LPM_EnableDeepSleep();
	SET_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);							//Set SLEEPDEEP bit of Cortex System Control Register

	//Request Wait For Interrupt
	__WFI();

	//Request Wait For Event
/*	__SEV();
	__WFE();
	__WFE();
*/
	//TODO: is it necessary?
//	CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);							//Reset SLEEPDEEP bit after exiting from Stop
}


void enterStandby(void)
{
	uart2SendText("going to StandBy, bye");
	while(HardwareEvents & HWE_UART2_TRANSMITTING);						//wait until the message transmits

	LL_PWR_ClearFlag_WU();
	LL_PWR_ClearFlag_SB();
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
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
