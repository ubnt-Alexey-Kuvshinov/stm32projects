/*
 *  Created on: Dec 02, 2016
 *      Author: Alexey
 */

#include <stm32L051x_board.h>

#define COMMAND_SIZE	6

typedef enum {
	CC_INITIAL,																		//Configure interface speed , image size
	CC_GET_PICTURE,																	//Get image from the module
	CC_SNAPSHOT,																	//Capture a still image
	CC_SET_PACKAGE_SIZE,															//Set the package size to transmit data from module to the host
	CC_RESET,																		//Reset the whole system
	CC_DATA_LENGTH,																	//Provide the data length information
	CC_SYNC,																		//Start a synchronization sequence
	CC_ACK,																			//Indicates the communication success
	CC_NAK,																			//Indicates the communication fail with error code
} CameraCommands;

typedef enum {
	CSMC_START,																		//reset and start receiving a new command
	CSMC_UPDATE,																	//process a newly received character
	CSMC_GET_COMMAND,																//get the last received command code
	CSMC_STOP,																		//stop receiving a command
} CommandSmCommands;

typedef enum {
	CSMS_IDLE,																		//not in the process of command reception
	CSMS_WAITING_START,																//waiting for 0xAA
	CSMS_RECEIVING,																	//getting cmd characters
} CommandSmStates;


typedef enum {
	PSMC_START,																		//reset and start receiving new picture
	PSMC_UPDATE,																	//process a newly received character
	PSMC_GET_DATA,																	//get the last received data
} PictureSmCommands;

typedef enum {
	PSMS_IDLE,																		//not in the process of command reception
	PSMS_RECEIVING_ID_1,															//getting first packet ID byte
	PSMS_RECEIVING_ID_2,															//getting second packet ID byte
	PSMS_RECEIVING_SIZE_1,															//getting first packet size byte
	PSMS_RECEIVING_SIZE_2,															//getting second packet size byte
	PSMS_RECEIVING_DATA,															//getting packet data
	PSMS_RECEIVING_VC_1,															//getting first byte of packet verification code
	PSMS_RECEIVING_VC_2,															//getting second byte of packet verification code
	PSMS_DONE,																		//a packet is fully received
	PSMS_FAILED,																	//error during packet reception
} PictureSmStates;

typedef enum {
	CCC_INITIAL				= 0x01,													//Configure interface speed , image size
	CCC_GET_PICTURE			= 0x04,													//Get image from the module
	CCC_SNAPSHOT			= 0x05,													//Capture a still image
	CCC_SET_PACKAGE_SIZE	= 0x06,													//Set the package size to transmit data from module to the host
	CCC_RESET				= 0x08,													//Reset the whole system
	CCC_DATA_LENGTH			= 0x0A,													//Provide the data length information
	CCC_SYNC				= 0x0D,													//Start a synchronization sequence
	CCC_ACK					= 0x0E,													//Indicates the communication success
	CCC_NAK					= 0x0F,													//Indicates the communication fail with error code
	CCC_START				= 0xAA,													//command start symbol
	CCC_NONE				= 0xFF,													//no valid command was received
} CameraCommandCodes;

typedef enum {
	CCP_START,																		//position of start symbol in the command
	CCP_CODE,																		//position of the command code in the command
	CCP_PARAM_1,																	//position of first parameter in the command
	CCP_PARAM_2,																	//position of second parameter in the command
	CCP_PARAM_3,																	//position of third parameter in the command
	CCP_PARAM_4,																	//position of fourth parameter in the command
} CameraCommandParts;

typedef struct {
	union {
		struct  {
			uint8_t packetNumberLsb;												//image packet LSByte
			uint8_t packetNumberMsb;												//image packet MSByte
		};
		uint16_t packetNumber;														//image packet number
	};

	uint16_t packetsNumber;															//total number of packages in an image
} imageTransfer;

imageTransfer packetsTracker;

uint8_t commands[][COMMAND_SIZE] =
						{{CCC_START, CCC_INITIAL, 0x04, 0x07, 0x00, 0x05},			//INITIAL			0x01, Interface Speed, 0x07, 0x00, 07-VGA / 05-QVGA
						 {CCC_START, CCC_GET_PICTURE, 0x01, 0x00, 0x00, 0x00},		//Get Picture		0x04, 0x01, 0x00, 0x00, 0x00
						 {CCC_START, CCC_SNAPSHOT, 0x00, 0x00, 0x00, 0x00},			//Snapshot			0x05, 0x00, 0x00, 0x00, 0x00
						 {CCC_START, CCC_SET_PACKAGE_SIZE, 0x08, 0x80, 0x00, 0x00},	//Set Package Size	0x06, 0x08, Package Size Low Byte, Package Size High Byte, 0x00
						 {CCC_START, CCC_RESET, 0x00, 0x00, 0x00, 0xFF},			//Reset				0x08, 0x00, 0x00, 0x00, Reset Priority
						 {CCC_START, CCC_DATA_LENGTH, 0x01, 0x00, 0x00, 0x00},		//DATA LENGTH		0x0A, 0x01, Length Byte 0, Length Byte 1, Length Byte 2
						 {CCC_START, CCC_SYNC, 0x00, 0x00, 0x00, 0x00},				//SYNC				0x0D, 0x00, 0x00, 0x00, 0x00
						 {CCC_START, CCC_ACK, 0x00, 0x00, 0x00, 0x00},				//ACK				0x0E, Command ID, ACK counter, 0x00/Package ID Byte0, 0x00/Package ID Byte 1
						 {CCC_START, CCC_NAK, 0x00, 0x00, 0x00, 0x00}};				//NAK				0x0F, Command ID, NAK counter, Error Code, 0x00

uint8_t cam_command[COMMAND_SIZE];													//last command received from camera

static uint16_t dataSize;
static PictureSmStates PictureSmstate = PSMS_IDLE;



static CameraCommandCodes getCommand(CommandSmCommands cmd)
{
	static CommandSmStates state = CSMS_IDLE;
	CameraCommandCodes ret = CCC_NONE;

	switch(cmd) {
	case CSMC_START:
		cam_command[CCP_CODE] = CCC_NONE;
		USARTx_RX_INTERRUPT_DISABLE(USART_2);										//disable receiver
		Uart2RxCounter = 0;
		USARTx_RX_INTERRUPT_ENABLE(USART_2);										//enable receiver
		state = CSMS_WAITING_START;
		break;
	case CSMC_UPDATE:
		USARTx_RX_INTERRUPT_DISABLE(USART_2);										//disable receiver
		switch(state) {
		case CSMS_WAITING_START:
			if(CCC_START == *Uart2RxBuffer)
				state = CSMS_RECEIVING;
			else
				Uart2RxCounter = 0;
			break;
		case CSMS_RECEIVING:
			if(COMMAND_SIZE <= Uart2RxCounter) {									//store the received command and start receiving next one
				memcpy(cam_command, Uart2RxBuffer, COMMAND_SIZE);
				Uart2RxCounter = 0;
				state = CSMS_WAITING_START;
				ret = cam_command[CCP_CODE];
			}
			break;
		}
		USARTx_RX_INTERRUPT_ENABLE(USART_2);										//enable receiver
		break;
	case CSMC_GET_COMMAND:
		ret = cam_command[CCP_CODE];
		cam_command[CCP_CODE] = CCC_NONE;											//return command code after which the command is considered reset
		break;
	case CSMC_STOP:

		break;
	}

	return ret;
}


//This function programs a half page. It is executed from the RAM.
__attribute__((section(".RamFunc"))) void FlashHalfPageProg(uint32_t flash_addr, uint32_t *data) {
	uint32_t count = 0;

    while(count++ < 16U)
      *(__IO uint32_t*) flash_addr = *data++;
}


PictureSmStates getPicture(PictureSmCommands cmd)
{
//	static PictureSmStates state = PSMS_IDLE;
//	static uint16_t dataSize;

	switch(cmd) {
	case PSMC_START:
		dataSize = 0;
		USARTx_RX_INTERRUPT_DISABLE(USART_2);										//disable receiver
		Uart2RxCounter = 0;
		USARTx_RX_INTERRUPT_ENABLE(USART_2);										//enable receiver
		PictureSmstate = PSMS_RECEIVING_ID_1;
		break;
	case PSMC_UPDATE:
		USARTx_RX_INTERRUPT_DISABLE(USART_2);										//disable receiver
		switch(PictureSmstate) {
		case PSMS_RECEIVING_ID_1:
			PictureSmstate = PSMS_RECEIVING_ID_2;
			break;
		case PSMS_RECEIVING_ID_2:
			PictureSmstate = PSMS_RECEIVING_SIZE_1;
			break;
		case PSMS_RECEIVING_SIZE_1:
			PictureSmstate = PSMS_RECEIVING_SIZE_2;
			break;
		case PSMS_RECEIVING_SIZE_2:
			dataSize = (Uart2RxBuffer[3] << 8) | Uart2RxBuffer[2];
			if((0 < dataSize) && (dataSize <= 128 - 6))
				PictureSmstate = PSMS_RECEIVING_DATA;
			else
				PictureSmstate = PSMS_FAILED;
			break;
		case PSMS_RECEIVING_DATA:
			if(Uart2RxCounter - 4 >= dataSize)
				PictureSmstate = PSMS_RECEIVING_VC_1;
			break;
		case PSMS_RECEIVING_VC_1:
			PictureSmstate = PSMS_RECEIVING_VC_2;
			break;
		case PSMS_RECEIVING_VC_2:
			PictureSmstate = PSMS_DONE;
			break;
		case PSMS_DONE:
			PictureSmstate = PSMS_FAILED;
			break;
		}
		USARTx_RX_INTERRUPT_ENABLE(USART_2);										//enable receiver
		break;
	case PSMC_GET_DATA: {
		uint8_t i;
		uint8_t chksum;

		USARTx_RX_INTERRUPT_DISABLE(USART_2);										//disable receiver making sure picture data is unchanged (though it's not supposed to)
		startUart1Msg(RC_PICTURE_DATA);												//move data to computer

		for(i = chksum = 0; i < dataSize + 4; i++) {								//calculate image packet check sum
			addToUart1Msg(Uart2RxBuffer[i]);
			chksum += Uart2RxBuffer[i];
		}

		if(chksum != Uart2RxBuffer[i])
			boardRedLedOn();
		else {
			addToUart1Msg(Uart2RxBuffer[i++]);										//copy image packet
			addToUart1Msg(Uart2RxBuffer[i]);
			sendUart1Message();
		}

	    __disable_irq();															//Disable all IRQs to save the image packet into FLASH

	    uint32tmp = 0x08008000 + ((Uart2RxBuffer[1] << 8) | Uart2RxBuffer[0]) * FLASH_PAGE_SIZE;

	    FLASH->PECR &= ~FLASH_PECR_FPRG;
		FLASH->PECR |= FLASH_PECR_ERASE;
	    *(__IO uint32_t *)uint32tmp = (uint32_t)0;
	    while ((FLASH->SR & FLASH_SR_BSY) != 0);
	    FLASH->PECR &= ~FLASH_PECR_ERASE;
	    FLASH->PECR |= FLASH_PECR_FPRG;

		FlashHalfPageProg(uint32tmp, Uart2RxBuffer);
		FlashHalfPageProg(uint32tmp + FLASH_PAGE_SIZE / 2, Uart2RxBuffer + FLASH_PAGE_SIZE / 2);

	    __enable_irq();																//Enable IRQs

		USARTx_RX_INTERRUPT_ENABLE(USART_2);										//enable receiver
		}
		break;
	}

	return PictureSmstate;
}


static void toggleCommSpeed()
{
	if(US_USART_14400 == USART_ptr(USART_2)->BRR)
		configureUart(USART_2, US_USART_115200);									//configure UART communication parameters 115200
	else
		configureUart(USART_2, US_USART_14400);										//configure UART communication parameters 115200
}


uint32_t CameraTask(CameraTaskCommands command, uint32_t parameter)
{
	static CameraTaskStatus state = CTS_IDLE;
	static uint8_t synchAttempts;

	switch(command) {

	case CTC_RESET:																	//resets radio task to idle, even if the task is in progress
		state = CTS_IDLE;
		break;

	case CTC_START:																	//start new picture acquisition
		if(CTS_IDLE != state)
			return CTS_FAILED;

		if(parameter)
			commands[CC_INITIAL][CCP_PARAM_4] = 0x07;								//get VGA picture
		else
			commands[CC_INITIAL][CCP_PARAM_4] = 0x05;								//get QVGA picture

		DeviceData.pictureSize = 0;													//this means the device does not have a picture

		//unlock FLASH for storing image into
		while ((FLASH->SR & FLASH_SR_BSY) != 0);									//Unlock the data EEPROM and FLASH_PECR register

		if ((FLASH->PECR & FLASH_PECR_PELOCK) != 0) {								//Check if the PELOCK is unlocked
			FLASH->PEKEYR = FLASH_PEKEY1;											//Perform unlock sequence
			FLASH->PEKEYR = FLASH_PEKEY2;
		}

		if (((FLASH->PECR & FLASH_PECR_PELOCK) == 0) && ((FLASH->PECR & FLASH_PECR_PRGLOCK) != 0)) {	//Unlock the NVM program memory
			FLASH->PRGKEYR = FLASH_PRGKEY1;											//Perform unlock sequence
			FLASH->PRGKEYR = FLASH_PRGKEY2;
		}

		FLASH->PECR |= FLASH_PECR_PROG | FLASH_PECR_FPRG;							//unlock the flash

		//initiate picture getting process
		synchAttempts = 0;
		commands[CC_ACK][CCP_PARAM_2] = commands[CC_ACK][CCP_PARAM_3] = commands[CC_ACK][CCP_PARAM_4] = 0;
		getCommand(CSMC_START);														//start command tracking state machine
		setTimer(CAMERA_SYNCH_DELAY, HWE_CAMERA_TIMEOUT);							//wait for an ACK to SYNCH
		state = CTS_SENDING_SYNC;
		break;

	case CTC_GET_STATE:
		return state;

	case CTC_GET_PWR_NEED:															//request for the task's minimum power requirement to determine power saving mode
		switch (state) {
		case CTS_IDLE:
		case CTS_SUCCESS:
		case CTS_FAILED:
			return PWR_STANDBY;
		default:
			return PWR_SLEEP;
		}
		break;

	case CTC_EVENT:
		CLEAR_EVENT(parameter);
		switch(parameter) {

		case HWE_CAMERA_TIMEOUT:
			switch (state) {
			case CTS_SENDING_SYNC:													//waiting for synchronization reply from the camera
			case CTS_SENDING_SYNC_ALTERNATE:										//waiting for synchronization reply from the camera on alternative baud rate
				if(++synchAttempts > CAMERA_SYNC_RETRIES) {
					if(CTS_SENDING_SYNC == state) {
						toggleCommSpeed();
						synchAttempts = 0;
						state = CTS_SENDING_SYNC_ALTERNATE;
					} else {
						sendUart2Message(commands[CC_RESET], COMMAND_SIZE);			//send RESET command
						setTimer(CAMERA_RESET_DELAY, HWE_CAMERA_TIMEOUT);			//fail the getting picture attempt
						state = CTS_RESET;
						break;
					}
				}

				sendUart2Message(commands[CC_SYNC], COMMAND_SIZE);					//resend synch request
				setTimer(CAMERA_SYNCH_DELAY, HWE_CAMERA_TIMEOUT);					//wait for an ACK to SYNCH
				break;
			case CTS_WAITING_INITIAL_ACK:											//waiting for acknowledge to INITIAL command
			case CTS_WAITING_PKTSIZE_ACK:											//waiting for acknowledge to PKTSIZE command
			case CTS_WAITING_SNAPSHOT_ACK:											//waiting for acknowledge to SNAPSHOT command
			case CTS_WAITING_DATA_LENGTH:											//waiting for image size after GET PICTURE command
			case CTS_GETTING_PICTURE:												//timeout receiving image data
				sendUart2Message(commands[CC_RESET], COMMAND_SIZE);					//send RESET command
				setTimer(CAMERA_RESET_DELAY, HWE_CAMERA_TIMEOUT);					//fail the getting picture attempt
				state = CTS_RESET;
				break;
			case CTS_AFTER_INITIAL_DELAY:											//delay after successful acknowledge to INITIAL command
				sendUart2Message(commands[CC_SET_PACKAGE_SIZE], COMMAND_SIZE);		//set package size to 128 bytes
				setTimer(CAMERA_INITIAL_DELAY, HWE_CAMERA_TIMEOUT);					//set PKTSIZE ACK time out
				state = CTS_WAITING_PKTSIZE_ACK;
				break;
			case CTS_RESET:
				state = CTS_FAILED;
				FLASH->PECR &= ~(FLASH_PECR_PROG | FLASH_PECR_FPRG);				//lock FLASH back
				break;
			}	//switch (state)
			break;

			case HWE_UART2_RX_EVENT:
				switch (state) {
				case CTS_SENDING_SYNC:												//waiting for synchronization reply from the camera
				case CTS_SENDING_SYNC_ALTERNATE:									//waiting for synchronization reply from the camera on alternative baud rate
					if(CCC_ACK == getCommand(CSMC_UPDATE))
				 		state = CTS_WAITING_SYNC;
					break;
				case CTS_WAITING_SYNC:												//waiting for synchronization reply from the camera
					if(CCC_SYNC == getCommand(CSMC_UPDATE)) {
						sendUart2Message(commands[CC_ACK], COMMAND_SIZE);			//send ACK
						sendUart2Message(commands[CC_INITIAL], COMMAND_SIZE);		//send INITIAL
						//while(HardwareEvents & HWE_UART2_TRANSMITTING);			//TODO: implement transmission complete event
						//configureUart(USART_2, US_USART_115200);					//set uart speed
						//getCommand(CSMC_START);									//start receiving reply on the new speed
						setTimer(CAMERA_INITIAL_DELAY, HWE_CAMERA_TIMEOUT);			//set ACK from camera time out
						state = CTS_WAITING_INITIAL_ACK;
					}
					break;
				case CTS_WAITING_INITIAL_ACK:										//waiting for INITIAL command acknowledge from the camera
					if(CCC_ACK == getCommand(CSMC_UPDATE)) {
						configureUart(USART_2, US_USART_115200);					//set uart speed to match that in INITIAL command
						setTimer(CAMERA_INITIAL_DELAY, HWE_CAMERA_TIMEOUT);			//delay next command
						state = CTS_AFTER_INITIAL_DELAY;
					}
					break;
				case CTS_WAITING_PKTSIZE_ACK:										//waiting for SET PKTSIZE command acknowledge from the camera
					if(CCC_ACK == getCommand(CSMC_UPDATE)) {
						sendUart2Message(commands[CC_SNAPSHOT], COMMAND_SIZE);		//send SNAPSHOT command
						setTimer(CAMERA_PICTURE_TIMEOUT, HWE_CAMERA_TIMEOUT);		//ACK time out
						state = CTS_WAITING_SNAPSHOT_ACK;
					}
					break;
				case CTS_WAITING_SNAPSHOT_ACK:										//waiting for SNAPSHOT command acknowledge from the camera
					if(CCC_ACK == getCommand(CSMC_UPDATE)) {
						sendUart2Message(commands[CC_GET_PICTURE], COMMAND_SIZE);	//send GET_PICTURE command
						setTimer(CAMERA_INITIAL_DELAY, HWE_CAMERA_TIMEOUT);			//delay next command
						state = CTS_WAITING_PICTURE_ACK;
					}
					break;
				case CTS_WAITING_PICTURE_ACK:										//waiting for GET_PICTURE command acknowledge from the camera
					if(CCC_ACK == getCommand(CSMC_UPDATE)) {
						setTimer(CAMERA_PICTURE_TIMEOUT, HWE_CAMERA_TIMEOUT);		//delay next command
						state = CTS_WAITING_DATA_LENGTH;
					}
					break;
				case CTS_WAITING_DATA_LENGTH:										//waiting for DATA_LENGTH command from the camera
					if(CCC_DATA_LENGTH == getCommand(CSMC_UPDATE)) {
						//do something
						//Number of package = Image size / (Package size - 6)
						packetsTracker.packetNumber = (cam_command[CCP_PARAM_3] << 8) | cam_command[CCP_PARAM_2];
						packetsTracker.packetsNumber = packetsTracker.packetNumber / (128 - 6);
						if(packetsTracker.packetNumber % (128 - 6))
							packetsTracker.packetsNumber++;

						packetsTracker.packetNumber = 0;

						getPicture(PSMC_START);
						sendUart2Message(commands[CC_ACK], COMMAND_SIZE);			//send GET_PICTURE ACK command
						setTimer(CAMERA_PICTURE_TIMEOUT, HWE_CAMERA_TIMEOUT);		//camera should respond with data within this time
						state = CTS_GETTING_PICTURE;
					}
					break;
				case CTS_GETTING_PICTURE:											//receiving picture data from the camera
					switch(getPicture(PSMC_UPDATE)) {
					case PSMS_DONE:													//new packet received
						getPicture(PSMC_GET_DATA);									//transfer the packet content out
						setTimer(4*CAMERA_PICTURE_TIMEOUT, HWE_CAMERA_TIMEOUT);		//camera should respond with data within this time TODO: remove 4
						break;														//wait for next packet external request
					case PSMS_FAILED:
						//do nothing wait for timeout to go to reset and then failed state
						break;
					}
					break;
				}	//switch (state)
				break;
		}	//switch(parameter)
		break;	//case GTC_EVENT:

	case CTC_GET_NEXT_DATA:
		if(CTS_GETTING_PICTURE == state) {
			if(++packetsTracker.packetNumber >= packetsTracker.packetsNumber) {
				commands[CC_ACK][CCP_PARAM_3] = commands[CC_ACK][CCP_PARAM_4] = 0xF0;
				sendUart2Message(commands[CC_ACK], COMMAND_SIZE);					//send the final GET_PICTURE ACK command

				startUart1Msg(RC_END_PICTURE);										//signal transfer completion to external consumer
				sendUart1Message();

				DeviceData.pictureSize = packetsTracker.packetsNumber;				//the device has a picture (assuming QVGA)

				FLASH->PECR &= ~(FLASH_PECR_PROG | FLASH_PECR_FPRG);				//lock FLASH back
				state = CTS_SUCCESS;
			} else {																//initiate next packet reception
				getPicture(PSMC_START);
				commands[CC_ACK][CCP_PARAM_3] = packetsTracker.packetNumberLsb;
				commands[CC_ACK][CCP_PARAM_4] = packetsTracker.packetNumberMsb;
				setTimer(3*CAMERA_PICTURE_TIMEOUT, HWE_CAMERA_TIMEOUT);				//camera should respond with data within this time
				sendUart2Message(commands[CC_ACK], COMMAND_SIZE);					//send GET_PICTURE ACK for next packet
			}
		}
		break;

	}	//switch(command)

	return GTS_SUCCESS;
}
