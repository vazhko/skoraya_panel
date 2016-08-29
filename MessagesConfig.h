
#ifndef MESS_CONFIG_H
#define MESS_CONFIG_H


#define MAX_MESSAGES 6
#define MAX_BROADCAST_MESSAGES 1
#define  MAX_SEMAPHORES 4

typedef enum {
    //MES_ONE = 0,	
    MES_RX,
    MSG_SEC,
    MSG_EXT_220,
    MSG_INT_220
            
} MESSAGES;


typedef enum {
	SEM_IS_FLASH_COMMUNICATING,
    SEM_RX_DONE,
    SEM_WR_DONE,
    SEM_WR_ERR
} SEM;

#endif

