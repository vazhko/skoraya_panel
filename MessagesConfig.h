
#ifndef MESS_CONFIG_H
#define MESS_CONFIG_H


#define MAX_MESSAGES 8
#define MAX_BROADCAST_MESSAGES 1
#define  MAX_SEMAPHORES 4

typedef enum {
    MES_RX,
    MSG_SEC,
    MSG_EXT_220,
    MSG_INT_220,
    MSG_MANUAL_SIDE,
    MSG_MANUAL_BACK
            
            
} MESSAGES;


typedef enum {
	SEM_IS_FLASH_COMMUNICATING,
    SEM_RX_DONE,
    SEM_WR_DONE,
    SEM_WR_ERR
} SEM;

#endif

