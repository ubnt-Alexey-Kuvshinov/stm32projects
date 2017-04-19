/*
 * accelerometer.h
 *
 *  Created on: Jan 19, 2017
 *      Author: Alexey
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

/*
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
} AccelerometerStatus;

typedef enum {
	AC1_RST				= 0b00000100,					//This bit is used to activate the software reset
	AC1_OST				= 0b00000010,					//In one-shot mode, when SBYB is 0, the OST bit is an auto-clear bit. When OST is set, the device initiates a measurement
														//by going into active mode. Once a pressure/altitude and temperature measurement is completed, it clears the OST bit and
														//comes back to STANDBY mode. User shall read the value of the OST bit before writing to this bit again.
	AC1_SBYB			= 0b00000001,					//This bit is sets the mode to ACTIVE, where the system will make measurements at periodic times based on the value of ST bits
} AccelerometerControl1;
*/

#define	ACCELEROMETER_I2C_ADDRESS	0x30

typedef enum {
	AR_STATUS_REG_AUX	= 0x07,							//temperature reading availability
	AR_OUT_TEMP_L		= 0x0C | 0x80,					//0x80 allows register address autoincrement for multiple reads/writes
	AR_OUT_TEMP_H		= 0x0D | 0x80,
	AR_TEMP_CFG_REG		= 0x1F | 0x80,					//0b00000000 - temperature sensor disabled, 0b11000000 - enabled
	AR_CTRL_REG1 		= 0x20 | 0x80,					//output data rate, Low-power mode enable, x, y and z axises enable bits
	AR_CTRL_REG2 		= 0x21 | 0x80,					//filters enables
	AR_CTRL_REG3 		= 0x22 | 0x80,					//interrupts enables on INT1 pin
	AR_CTRL_REG4 		= 0x23 | 0x80,					//Block data update, Big/Little Endian, Full-scale selection, Operating mode selection, Self-test enable, SPI mode selection
	AR_CTRL_REG5 		= 0x24 | 0x80,					//BOOT, FIFO_EN, --, --, LIR_INT1, D4D_INT1, LIR_INT2, D4D_INT2
	AR_STATUS_REG		= 0x27 | 0x80,					//X, Y, Z data availability and status
	AR_OUT_X_L			= 0x28 | 0x80,					//X acceleration data
	AR_OUT_Y_L			= 0x2A | 0x80,					//Y acceleration data
	AR_OUT_Z_L			= 0x2C | 0x80,					//Z acceleration data
	AR_INT1_CFG 		= 0x30 | 0x80,					//X, Y, Z high/low interrupts enables, AOI and 6D combinations
	AR_INT1_SRC 		= 0x31 | 0x80,					//status of INT1 interrupts
	AR_INT1_THS 		= 0x32 | 0x80,					//status of the above interrupts
	AR_INT2_SRC 		= 0x35 | 0x80,					//status of INT2 interrupts
} AccelerometerRegisters;

typedef enum {
	ATS_IDLE,											//resets task to idle, even if the task is in progress
	ATS_READING_DATA,									//accessing altimeter chip via I2C
	ATS_SUCCESS,
	ATS_FAILED											//indicates failure of a command
} AccelerometerTaskStatus;

typedef enum {
	ATC_RESET,											//resets task to idle, even if the task is in progress
	ATC_START,											//start new data acquisition
	ATC_GET_STATE,										//request for the task progress/state
	ATC_GET_PWR_NEED,									//request for the task's minimum power requirement to determine power saving mode
	ATC_EVENT,											//hardware events related to altimetery, e.g timeout or interrupts
} AccelerometerTaskCommands;

typedef	struct {
	union {
		union {
			int16_t temperature_reading;				//value of AR_OUT_TEMP_L and AR_OUT_TEMP_H registers
			struct {
				uint8_t temperature_fractional;			//fractional part of the temperature in 0,25 Celcius
				uint8_t temperature_whole;				//whole part of temperature in Celcius
			};
		};
		uint8_t buffer[4];
	};
} AccelerometerData;

extern AccelerometerData accelerometerData;

void AccelerometerWriteBytes(uint8_t* data, uint8_t length);
uint32_t AccelerometerTask(AccelerometerTaskCommands command, uint32_t parameter);

#endif /* ACCELEROMETER_H_ */
