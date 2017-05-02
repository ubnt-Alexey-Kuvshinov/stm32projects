/*
 * accelerometer.c
 *
 *  Created on: Jan 17, 2017
 *      Author: Alexey
 */


#include <stm32L052x_board.h>

AccelerometerData accelerometerData;

void AccelerometerWriteBytes(uint8_t *data, uint8_t length)
{
	LL_I2C_Enable(I2C1);
	i2cWrite(ACCELEROMETER_I2C_ADDRESS, data, length, 1);
	LL_I2C_Disable(I2C1);
}


static uint8_t AccelerometerReadBytes(uint8_t address, uint8_t* data, uint8_t length)
{
	*data = address;
	LL_I2C_Enable(I2C1);
    if(i2cWrite(ACCELEROMETER_I2C_ADDRESS, data, 1, 0)) {											//successful write
    	i2cRead(ACCELEROMETER_I2C_ADDRESS, data, length);
    } else {
    	LL_I2C_Disable(I2C1);
    	return 0;
    }
	LL_I2C_Disable(I2C1);
    return 1;
}


uint32_t AccelerometerTask(AccelerometerTaskCommands command, uint32_t parameter)
{
	static AccelerometerTaskStatus state = ATS_IDLE;

	static uint32_t regAddress;

	switch(command) {
	case ATC_RESET:														//resets task to idle, even if the task is in progress TODO: what about scheduled timeouts then?
		state = ATS_IDLE;
		break;
	case ATC_START:														//start temperature acquisition
		if(ATS_IDLE != state)
			return ATS_FAILED;

		regAddress = parameter;

		if(AccelerometerReadBytes(AR_OUT_TEMP_L, accelerometerData.buffer, 2)) {	//discard current temperature register content
			setTimer(timer_milliseconds(200), HWE_ACCEL_TIMEOUT);		//wait for fresh temperature reading
			state = ATS_READING_DATA;
		} else {
			state = ATS_FAILED;
		}
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
	case ATC_EVENT:
		CLEAR_EVENT(parameter);
		switch(parameter) {
		case HWE_ACCEL_TIMEOUT:
			switch (state) {
			case ATS_READING_DATA:
				//if(AccelerometerReadBytes(AR_OUT_TEMP_L, accelerometerData.buffer, 2)) {
				//if(AccelerometerReadBytes(AR_INT1_CFG, accelerometerData.buffer, 2)) {
				if(AccelerometerReadBytes(regAddress, accelerometerData.buffer, 4)) {
					//calculate temperature
					accelerometerData.temperature_reading >>= 6;		//getting 10-bit value of 0,25C units
					if(accelerometerData.temperature_reading & 0b1000000000) {
						accelerometerData.temperature_reading -=1;		//calculate negative value
						accelerometerData.temperature_reading = ~accelerometerData.temperature_reading & 0x3F;
						accelerometerData.temperature_reading = -accelerometerData.temperature_reading;
					}
					accelerometerData.temperature_reading += 48;		//get absolute temperature from the 12C difference read by the sensor
					state = ATS_SUCCESS;
				} else {
					state = ATS_FAILED;
				}
				break;
			}
			break;
		}
	}

	return ATS_SUCCESS;
}
