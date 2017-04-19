#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_BUFFER_SIZE			128

#define CAMERA_SYNCH_DELAY			timer_milliseconds(10)		//wait time before checking for Ack after sending SYNCH command
#define CAMERA_INITIAL_DELAY		timer_milliseconds(50)		//wait time before next command can follow INITIAL command
#define CAMERA_PICTURE_TIMEOUT		timer_milliseconds(200)		//max time for response to GET PICTURE or GET DATA commands
//#define CAMERA_SNAPSHOT_TIMEOUT		timer_milliseconds(2000)	//max time for response to SNAPSHOT command
#define CAMERA_RESET_DELAY			timer_milliseconds(20)		//wait time before next command can follow RESET command
#define CAMERA_SYNC_RETRIES			60							//max number of times to send SYNC command before Ack is received


typedef enum {
	CTS_IDLE,													//initial state
	CTS_SENDING_SYNC,											//first step in image acquisition,- sending SYNC command until Acked or 60 retries
	CTS_SENDING_SYNC_ALTERNATE,									//if CTS_SENDING_SYNC does not succeed, retry with another baud rate
	CTS_WAITING_SYNC,											//after initial Ack the camera should send SYNC
	CTS_WAITING_INITIAL_ACK,									//waiting for acknowledge of the INITIAL command
	CTS_AFTER_INITIAL_DELAY,									//waiting for 50ms before sending next command
	CTS_WAITING_PKTSIZE_ACK,									//waiting for acknowledge of the SET PACKET SIZE command
	CTS_WAITING_SNAPSHOT_ACK,									//waiting for acknowledge of the SNAPSHOT command
	CTS_WAITING_PICTURE_ACK,									//waiting for acknowledge of the GET PICTURE command
	CTS_WAITING_DATA_LENGTH,									//waiting up to 200ms for image size report after getting GET PICTURE ACK
	//	CTS_AFTER_PICTURE_DELAY,									//waiting for 200ms after getting GET PICTURE ACK
	CTS_GETTING_PICTURE,										//reading the picture data
	CTS_RESET,													//waiting before starting new sequence
	CTS_SUCCESS,
	CTS_FAILED
} CameraTaskStatus;

typedef enum {
	CTC_RESET,													//resets task to idle, even if the task is in progress
	CTC_START,													//start new picture acquisition process
	CTC_GET_STATE,												//report current task state
	CTC_IS_FINISHED,											//report if the task finished acquisition process, successfully or not
	CTC_GET_PWR_NEED,											//request for the task's minimum power requirement to determine power saving mode
	CTC_EVENT,													//hardware events related to the task e.g. timeout or interrupt
	CTC_GET_NEXT_DATA,											//command from external picture consumer to get next picture packet
} CameraTaskCommands;


uint32_t CameraTask(CameraTaskCommands command, uint32_t parameter);

#endif	//CAMERA_H
