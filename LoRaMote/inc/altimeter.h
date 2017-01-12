/*
 * altimeter.h
 *
 *  Created on: Nov 14, 2016
 *      Author: Alexey
 */

#ifndef ALTIMETER_H_
#define ALTIMETER_H_

//provides the acquisition status information on a per sample basis, and reflects real-time updates to the OUT_P and OUT_T registers
typedef enum {
	AS_PTOW				= 0b10000000,
	AS_POW				= 0b01000000,
	AS_TOW				= 0b00100000,
	AS_RESERVED4		= 0b00010000,
	AS_PTDR				= 0b00001000,
	AS_PDR				= 0b00000100,
	AS_TDR				= 0b00000010,
	AS_RESERVED0		= 0b00000001,
} AltimeterStatus;

typedef enum {
	AC1_RST				= 0b00000100,					//This bit is used to activate the software reset
	AC1_OST				= 0b00000010,					//In one-shot mode, when SBYB is 0, the OST bit is an auto-clear bit. When OST is set, the device initiates a measurement
														//by going into active mode. Once a pressure/altitude and temperature measurement is completed, it clears the OST bit and
														//comes back to STANDBY mode. User shall read the value of the OST bit before writing to this bit again.
	AC1_SBYB			= 0b00000001,					//This bit is sets the mode to ACTIVE, where the system will make measurements at periodic times based on the value of ST bits
} AltimeterControl1;

typedef enum {
	AR_I2C_ADDRESS		= 0x60,
	AR_STATUS			= 0x00,
	AR_TEMPERATURE_MSB	= 0x04,
	AR_TEMPERATURE_LSB	= 0x05,
	AR_ID				= 0x0C,
	AR_CONTROL1			= 0x26,
} AltimeterRegisters;

typedef enum {
	ATS_IDLE,											//resets task to idle, even if the task is in progress
	ATS_READING_DATA,									//accessing altimeter chip via I2C
	ATS_SUCCESS,
	ATS_FAILED											//indicates failure of a command
} AltimeterTaskStatus;

typedef enum {
	ATC_RESET,											//resets task to idle, even if the task is in progress
	ATC_START,											//start new data acquisition
	ATC_GET_STATE,										//request for the task progress/state
	ATC_GET_PWR_NEED,									//request for the task's minimum power requirement to determine power saving mode
	ATC_EVENT,											//hardware events related to altimetery, e.g timeout or interrupts
} AltimeterTaskCommands;

typedef	struct {
	union {
		struct {
			uint8_t temperature_whole;					//whole part of temperature in Celcius
			uint8_t temperature_fractional;				//fractional part of the temperature
		};
		uint8_t temperature[2];
	};
} altimeterData;

extern altimeterData AltimeterData;

uint8_t i2cReadMPL3115A(uint8_t a);
void i2cWriteMPL3115A(uint8_t a, uint8_t d);
uint32_t AltimeterTask(AltimeterTaskCommands command, uint32_t parameter);

#endif /* ALTIMETER_H_ */
