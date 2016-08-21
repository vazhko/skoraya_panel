
#ifndef MESS_CONFIG_H
#define MESS_CONFIG_H


#define MAX_MESSAGES 10
#define MAX_BROADCAST_MESSAGES 1
#define  MAX_SEMAPHORES 4

typedef enum {
    //MES_NONE = 0,	
    MES_RX   
} MESSAGES;


typedef enum {
	SEM_IS_FLASH_COMMUNICATING,
    SEM_RX_DONE,
    SEM_WR_DONE,
    SEM_WR_ERR
} SEM;

#endif

