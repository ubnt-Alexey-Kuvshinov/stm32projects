#ifndef GPS_H
#define GPS_H

#define GPS_BUFFER_SIZE		96

#define GPS_FIX_WAIT_BLINK_TIME		timer_milliseconds(800)		//GPS led blink frequency during the fix wait
#define GPS_FIX_TIMEOUT				8							//waiting for a fix time in blink intervals, keep GPS enabled for this many seconds before reading it's output, this gives the chip time to establish positioning with warm start (less than 30 mins from last fix)
#define GPS_READ_WAIT_BLINK_TIME	timer_milliseconds(300)		//GPS led blink frequency during data reading
#define GPS_READ_TIMEOUT			16							//GPS data read timeout in blinks, gives gps some time to report position over UART, needed for STM32L1xx as it has to be at full power for UART to work
#define GPS_FIX_TIMEOUT_WARM		(GPS_FIX_WAIT_BLINK_TIME * GPS_FIX_TIMEOUT + GPS_READ_WAIT_BLINK_TIME * GPS_READ_TIMEOUT) * 18	//approx 3 mins
#define GPS_FIX_RETRIES				30							//repeat this many attempts with GPS_FIX_TIMEOUT_WARM delay to get a fix after cold start (~5 minutes)


typedef enum {
	GTS_IDLE,													//initial state
	GTS_WAITING_FOR_FIX,										//waiting for GPS chip to acquire position, keeping it enabled
	GTS_READING_FIX_RESULT,										//reading GPS UART reports looking for valid positioning data
	GTS_SUCCESS,
	GTS_FAILED													//indicates GPS failure to locate after cold-start timeout
} GpsTaskStatus;

typedef enum {
	GTC_RESET,													//resets GPS task to idle, even if the task is in progress
	GTC_START,													//start new position acquisition process
	GTC_GET_STATE,												//report current task state
	GTC_IS_FINISHED,											//report if the task finished acquisition process, successfully or not
	GTC_GET_PWR_NEED,											//request for the task's minimum power requirement to determine power saving mode
	GTC_EVENT,													//hardware events related to the task e.g. timeout or interrupt
} GpsTaskCommands;

typedef enum {
	GS_INVALID,
	GS_GPGGA,
	GS_GPRMC
}GpsStatus;



typedef struct {
	struct {
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
		struct
		{
			uint8_t latitudeFlag	: 1;	//equals 0x01 if latitude is Southern
			uint8_t longitudeFlag	: 1;	//equals 0x01 if longitude is Eastern
			uint8_t fixType			: 3;	//max value is 6
			uint8_t status			: 2;	//GpsStatus type value: Invalid or obtained from GPGGA or GPRMC
		};
	} time;

	struct {
		uint8_t degrees;
		uint8_t minutes;
		uint16_t minuteDecimals;
	} latitude;

	struct {
		uint8_t degrees;
		uint8_t minutes;
		uint16_t minuteDecimals;
	} longitude;

//	uint8_t latitudeFlag;				//equals 0x01 if latitude is Southern
//	uint8_t longitudeFlag;				//equals 0x01 if longitude is Eastern
//	uint8_t fixType;					//max value is 6
//	uint8_t status;						//GpsStatus type value: Invalid or obtained from GPGGA or GPRMC
} GPS;




extern uint8_t GpsRxBuffer[GPS_BUFFER_SIZE];
extern uint8_t GpsRxCounter;
extern GPS GpsData;

uint32_t GpsTask(GpsTaskCommands command, uint32_t parameter);

#endif	//GPS_H
