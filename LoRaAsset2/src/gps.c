/*
 *  Created on: Oct 25, 2016
 *      Author: Alexey
 */

#include <stm32L052x_board.h>

enum GPGGA
{
	GPGGA_MSG_ID = 0,
	GPGGA_TIME,
	GPGGA_LATITUDE,
	GPGGA_NORTH_SOUTH,
	GPGGA_LONGITUDE,
	GPGGA_EAST_WEST,
	GPGGA_FIX_TYPE,
	GPGGA_SATELLITES,
	GPGGA_HDOP,
	GPGGA_ALTITUDE,
	GPGGA_ALTITUDE_UNITS,
	GPGGA_GS,
	GPGGA_GS_UNITS,
	GPGGA_AGE_OF_DIFF,
	GPGGA_DIFF_REF,
	GPGGA_CHECKSUM,
	GPGGA_SIZE
};

enum GPRMC
{
	GPRMC_MSG_ID = 0,
	GPRMC_TIME,
	GPRMC_STATUS,
	GPRMC_LATITUDE,
	GPRMC_NORTH_SOUTH,
	GPRMC_LONGITUDE,
	GPRMC_EAST_WEST,
	GPRMC_SPEED_OVER_GROUND,
	GPRMC_COURSE_OVER_GROUND,
	GPRMC_DATE,
	GPRMC_MAGNETIC_VARIATION,
	GPRMC_MAGNETIC_EAST_WEST,
	GPRMC_MODE,
	GPRMC_CHECKSUM,
	GPRMC_SIZE
};


uint8_t GpsRxBuffer[GPS_BUFFER_SIZE];
uint8_t GpsRxCounter;
GpsUartTx Uart1Tx;
GPS GpsData;


char GPGGAFieldsMaxSize[GPGGA_SIZE] = {6, 11, 10, 2, 11, 2, 2, 3, 6, 8, 2, 8, 2, 4, 4, 3};	//not sure about last three values
char GPRMCFieldsMaxSize[GPRMC_SIZE] = {6, 11, 2, 10, 2, 11, 2, 8, 8, 8, 6, 2, 2, 3};		//not sure about last three values


uint8_t *GetNextField(uint8_t *input, int32_t *size)
{
	static uint8_t *next;
	uint8_t *field;

	if(input)					//first call
		next = input;

	field = next;
	*size = 0;

	while(*next)
	{
		(*size)++;
		if(',' == *next++) {
			break;
		}
	}

	if(*size)
		return field;
	else
		return 0;
}

//$GPGGA,144831.000,4338.9935,N,07923.0863,W,1,4,3.15,205.4,M, 35.1,M,,*67
int GpsParseGPGGAMessage(uint8_t *input)
{
	uint8_t *data;
	uint8_t numbers[12];

	uint8tmp = 1;
	while((data = GetNextField(0, &tmp)))
	{
		if(tmp > GPGGAFieldsMaxSize[uint8tmp])
			return 0;

		for(; tmp; tmp--)										//primitive conversion of characters to decimals they represent
			numbers[tmp - 1] = data[tmp - 1] & 0x0F;

		switch(uint8tmp++)
		{
		case GPGGA_TIME:
			GpsData.time.hours = numbers[0] * 10 + numbers[1];
			GpsData.time.minutes = numbers[2] * 10 + numbers[3];
			GpsData.time.seconds = numbers[4] * 10 + numbers[5];
			break;
		case GPGGA_LATITUDE:
			GpsData.latitude.degrees = numbers[0] * 10 + numbers[1];
			GpsData.latitude.minutes = numbers[2] * 10 + numbers[3];
			GpsData.latitude.minuteDecimals = numbers[5] * 1000 + numbers[6] * 100 + numbers[7] * 10 + numbers[8];	//skip numbers[4] which is the dot
			break;
		case GPGGA_NORTH_SOUTH:
			if('S' == data[0])
				GpsData.time.latitudeFlag = 0x01;
			else
				GpsData.time.latitudeFlag = 0x00;
			break;
		case GPGGA_LONGITUDE:
			GpsData.longitude.degrees = numbers[0] * 100 + numbers[1] * 10 + numbers[2];
			GpsData.longitude.minutes = numbers[3] * 10 + numbers[4];
			GpsData.longitude.minuteDecimals = numbers[6] * 1000 + numbers[7] * 100 + numbers[8] * 10 + numbers[9];	//skip numbers[5] which is the dot
			break;
		case GPGGA_EAST_WEST:
			if('E' == data[0])
				GpsData.time.longitudeFlag = 0x01;
			else
				GpsData.time.longitudeFlag = 0x00;
			break;
		case GPGGA_FIX_TYPE:
			GpsData.time.fixType = numbers[0];
			if(0 == GpsData.time.fixType)
				return 0;
			break;
		case GPGGA_SATELLITES:
		case GPGGA_HDOP:
		case GPGGA_ALTITUDE:									//not implemented for BW conservation and the field format obscurity
		case GPGGA_ALTITUDE_UNITS:
		case GPGGA_GS:
		case GPGGA_GS_UNITS:
		case GPGGA_AGE_OF_DIFF:
		case GPGGA_DIFF_REF:
		case GPGGA_CHECKSUM:
			GpsData.time.status = GS_GPGGA;
			break;
		default:
			return 0;
		}
	}
	return 1;
}

int GpsParseGPRMCMessage(uint8_t *input)
{
	uint8_t *data;
	uint8_t numbers[12];

	uint8tmp = 1;
	while((data = GetNextField(0, &tmp)))
	{
		if(tmp > GPGGAFieldsMaxSize[uint8tmp])
			return 0;

		for(; tmp; tmp--)										//primitive conversion of characters to decimals they represent
			numbers[tmp - 1] = data[tmp - 1] & 0x0F;

		switch(uint8tmp++)
		{
		case GPRMC_TIME:
			GpsData.time.hours = numbers[0] * 10 + numbers[1];
			GpsData.time.minutes = numbers[2] * 10 + numbers[3];
			GpsData.time.seconds = numbers[4] * 10 + numbers[5];
			break;
		case GPRMC_STATUS:
			if('A' != data[0])
				return 0;
			break;
		case GPRMC_LATITUDE:
			GpsData.latitude.degrees = numbers[0] * 10 + numbers[1];
			GpsData.latitude.minutes = numbers[2] * 10 + numbers[3];
			GpsData.latitude.minuteDecimals = numbers[4] * 1000 + numbers[5] * 100 + numbers[6] * 10 + numbers[7];
			break;
		case GPRMC_NORTH_SOUTH:
			if('S' == data[0])
				GpsData.time.latitudeFlag = 0x01;
			else
				GpsData.time.latitudeFlag = 0x00;
			break;
		case GPRMC_LONGITUDE:
			GpsData.longitude.degrees = numbers[0] * 100 + numbers[1] * 10 + numbers[2];
			GpsData.longitude.minutes = numbers[3] * 10 + numbers[4];
			GpsData.longitude.minuteDecimals = numbers[5] * 1000 + numbers[6] * 100 + numbers[7] * 10 + numbers[8];
			break;
		case GPRMC_EAST_WEST:
			if('E' == data[0])
				GpsData.time.longitudeFlag = 0x01;
			else
				GpsData.time.longitudeFlag = 0x00;
			break;
		case GPRMC_SPEED_OVER_GROUND:									//not implemented so far
		case GPRMC_COURSE_OVER_GROUND:
		case GPRMC_DATE:
		case GPRMC_MAGNETIC_VARIATION:
		case GPRMC_MAGNETIC_EAST_WEST:
		case GPRMC_MODE:
		case GPRMC_CHECKSUM:
			GpsData.time.status = GS_GPRMC;
			break;
		default:
			return 0;
		}
	}
	return 1;
}

//returns non-zero, success, if positioning data was received
int GpsCheckMessage(uint8_t *command)
{
	uint8_t *messageType;

	if( (messageType = GetNextField(command, &tmp))) {
		if(0 == strncmp( ( const char* )command, "$GPGGA", 6))
			return GpsParseGPGGAMessage(messageType);
		else if(0 == strncmp( ( const char* )command, "$GPRMC", 6))
			return GpsParseGPRMCMessage(messageType);
	}
		return 0;	//no useful data or parsing error
}


uint32_t GpsTask(GpsTaskCommands command, uint32_t parameter)
{
	static GpsTaskStatus state = GTS_IDLE;
	static uint8_t fixAttempts;
	static uint8_t blinkCycles;

	switch(command) {
	case GTC_RESET:														//resets radio task to idle, even if the task is in progress
		state = GTS_IDLE;
		break;
	case GTC_START:														//start new sequence: request transmission parameters from Control Channel - get reply - pack the data at scheduled time - transmit the data using parameters from CC
		if(GTS_IDLE != state)
			return GTS_FAILED;

		sendUart1Message("blia", 4);									//to exit from sleep mode
		fixAttempts = 0;
		blinkCycles = 0;
		setTimer(GPS_FIX_WAIT_BLINK_TIME, HWE_GPS_TIMEOUT);				//wait for a fix
		state = GTS_WAITING_FOR_FIX;
		break;
	case GTC_GET_STATE:													//request for the task progress/state
		return state;
	case GTC_GET_PWR_NEED:												//request for the task's minimum power requirement to determine power saving mode
		switch (state) {
		case GTS_IDLE:
		case GTS_SUCCESS:
		case GTS_FAILED:
			return PWR_STANDBY;
		case GTS_READING_FIX_RESULT:
			return PWR_FULL;
		default:
			return PWR_SLEEP;
		}
		break;
	case GTC_EVENT:
		CLEAR_EVENT(parameter);
		switch(parameter) {
		case HWE_GPS_TIMEOUT:
			switch (state) {
			case GTS_WAITING_FOR_FIX:
				boardWhiteLedToggle();
				if(++blinkCycles < GPS_FIX_TIMEOUT)
					setTimer(GPS_FIX_WAIT_BLINK_TIME, HWE_GPS_TIMEOUT);
				else {
					blinkCycles = 0;
					setTimer(GPS_READ_WAIT_BLINK_TIME, HWE_GPS_TIMEOUT);
//					HardwareEvents |= HWE_LPUART_RECEIVING;
					USARTx_RX_INTERRUPT_ENABLE(LPUART);
					state = GTS_READING_FIX_RESULT;
				}
				break;
			case GTS_READING_FIX_RESULT:
				boardWhiteLedToggle();
				if(++blinkCycles < GPS_READ_TIMEOUT)
					setTimer(GPS_READ_WAIT_BLINK_TIME, HWE_GPS_TIMEOUT);
				else {
//					CLEAR_EVENT(HWE_LPUART_RECEIVING);
					blinkCycles = 0;
					if(++fixAttempts < GPS_FIX_RETRIES) {
						setTimer(GPS_FIX_WAIT_BLINK_TIME, HWE_GPS_TIMEOUT);
						state = GTS_WAITING_FOR_FIX;
					} else {											//could not get position even after cold-start timeout
						boardWhiteLedOn();
						boardRedLedOn();
						state = GTS_FAILED;
						sendUart1Message("$PMTK161,0*28\r\n", 15);		//put GPS into low-power mode
					}
				}
				break;
			}
			break;
		case HWE_UART1_COMMAND:
			CLEAR_EVENT(HWE_UART1_COMMAND);
			switch (state) {
			case GTS_READING_FIX_RESULT:
//			default:
				if(GpsCheckMessage(GpsRxBuffer)) {
					boardGreenLedOn();
					boardWhiteLedOn();
					state = GTS_SUCCESS;
					sendUart1Message("$PMTK161,0*28\r\n", 15);			//put GPS into low-power mode TODO: turn GPS power off instead of the message sending
					//TODO: clear power keeping flag here, implement through the whole task
				} else
					USARTx_RX_INTERRUPT_ENABLE(USART_1);				//continue reading data from UART
				break;
			}
		}	//switch(parameter)
		break;	//case GTC_EVENT:
	}	//switch(command)

	return GTS_SUCCESS;
}
