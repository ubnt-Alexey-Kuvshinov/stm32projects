/*
 * radio.h
 *
 *  Created on: Oct 28, 2016
 *      Author: Alexey
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stm32L051x_board.h>

#include "../../SensorPrototypeLL/inc/radio_common.h"


typedef enum {
	RTS_IDLE,											//resets radio task to idle, even if the task is in progress
	RTS_ACTIVATING_RADIO,								//waiting for radio chip becoming ready after releasing it's reset line
	RTS_TRANSMITTING_CC_REQUEST,
	RTS_LISTENING_CC_REPLY,								//waiting for valid header from Control Channel
	RTS_RECEIVING_CC_REPLY,
	RTS_WAITING_FOR_DATA_SCHEDULE,						//waiting for the scheduled data transmission time, the channel parameters are known, data mining tasks can be running
	RTS_WAITING_FOR_DS_STANDBY,							//same as before, but can go to deep sleep, the channel parameters must be saved and wake up event from StandBy implemented TODO: implement
	RTS_TRANSMITTING_DATA,
	RTS_LISTENING_DATA_CONFIRMATION,					//waiting for valid header from Data Channel
	RTS_RECEIVING_DATA_CONFIRMATION,
	RTS_SUCCESS,
	RTS_FAILED											//indicates failure of a command
} RadioTaskStatus;

typedef enum {
	RTC_RESET,											//resets radio task to idle, even if the task is in progress
	RTC_START,											//start new sequence: request transmission parameters from Control Channel - get reply - pack the data at scheduled time - transmit the data using parameters from CC
	RTC_GET_STATE,										//request for the task progress/state
	RTC_GET_PWR_NEED,									//request for the task's minimum power requirement to determine power saving mode
	RTC_EVENT,											//hardware events related to radio, e.g timeout or radio interrupts
} RadioTaskCommands;

typedef union {
	struct  {
			uint32_t fq;								//frequency in 61.035Hz steps in reversed order
			uint8_t bw;									//bandwidth
			uint8_t sf;									//spreading factor
			uint8_t cr;									//coding rate
			uint8_t eh;									//explicit/implicit header
		};
	uint8_t settingsArray[8];
} RadioSettings;

void RadioSetCommunicationParameters(RadioSettings *rs);
void RadioTransmit();
void RadioReceive(void);

//parameter is an additional command - specific data that some commands may need, e.g. data transmission delay
uint32_t RadioTask(RadioTaskCommands command, uint32_t parameter);

#endif /* RADIO_H_ */
