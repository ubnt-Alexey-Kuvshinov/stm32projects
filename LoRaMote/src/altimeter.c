/*
 * altimeter.c
 *
 *  Created on: Nov 14, 2016
 *      Author: Alexey
 */


#include "stm32l152x_board.h"

altimeterData AltimeterData;

void i2cWriteMPL3115A(uint8_t a, uint8_t d)
{
	uint8_t cmd[2];

    cmd[0] = a;
    cmd[1] = d;

    i2cWrite(AR_I2C_ADDRESS, cmd, 2, 1);
}


uint8_t i2cReadMPL3115A(uint8_t a)
{
	uint8_t cmd[2];

    cmd[0] = a;
    i2cWrite(AR_I2C_ADDRESS, cmd, 1, 0);
    i2cRead(AR_I2C_ADDRESS, cmd, 1);

    return cmd[0];
}

uint32_t AltimeterTask(AltimeterTaskCommands command, uint32_t parameter)
{
	static AltimeterTaskStatus state = ATS_IDLE;

	switch(command) {
	case ATC_RESET:														//resets radio task to idle, even if the task is in progress
		state = ATS_IDLE;
		break;
	case ATC_START:														//start new sequence: request transmission parameters from Control Channel - get reply - pack the data at scheduled time - transmit the data using parameters from CC
		if(ATS_IDLE != state)
			return ATS_FAILED;
		else
			state = ATS_READING_DATA;


		i2cWriteMPL3115A(AR_CONTROL1, 0xB8);
		i2cWriteMPL3115A(0x13, 0x07);
		i2cWriteMPL3115A(AR_CONTROL1, 0xB9);

		do {
			uint8tmp = i2cReadMPL3115A(AR_STATUS);
		} while (!uint8tmp);											//TODO: implement the wait as timeout


//i2cWriteMPL3115A(AR_CONTROL1, AC1_OST | AC1_SBYB);							//request one-time measurement
//		do {
//			uint8tmp = i2cReadMPL3115A(AR_CONTROL1);
//		} while (uint8tmp & AC1_OST);

//		uint8tmp = i2cReadMPL3115A(AR_ID);
//		uint8tmp = i2cReadMPL3115A(AR_TEMPERATURE_MSB);
		*AltimeterData.temperature = AR_TEMPERATURE_MSB;
	    i2cWrite(AR_I2C_ADDRESS, AltimeterData.temperature, 1, 0);
	    i2cRead(AR_I2C_ADDRESS, AltimeterData.temperature, 2);

	    state = ATS_SUCCESS;
		break;
	case ATC_GET_STATE:													//request for the task progress/state
		return state;
	case ATC_GET_PWR_NEED:												//request for the task's minimum power requirement to determine power saving mode
		switch (state) {
		case ATS_IDLE:
		case ATS_SUCCESS:
		case ATS_FAILED:
			return PWR_STANDBY;
		default:
			return PWR_SLEEP;
		}
		break;
	}

	return ATS_SUCCESS;
}
