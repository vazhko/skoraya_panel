#include "HardwareProfile.h"


#ifndef MAX_MESSAGES
#include "MessagesConfig.h"
#endif


typedef enum {
	MSG_PHASE_NONE,
	MSG_PHASE_PREPARE,	   
	MSG_PHASE_READY	
	
} MSG_PHASES;



extern void InitMessages(void);

// прием сообщения
extern char GetMessage(char Msg);
// проверка сообщения без сбрасывания
extern char ReadyMessage(char Msg);

extern char GetBroadcastMessage(char Msg);

extern void SendMessage(char Msg);
extern void SendBroadcastMessage(char Msg);

// Using 1 parameter for messegies
// Need to 
// #define USE_MESSAGE_PARAM

#ifdef USE_MESSAGE_PARAM
	char GetMessageP(char Msg, char *pParam);
	void SendMessageP(char Msg, char Param)	;
#endif

// Binary semaphores
extern void BSemOn(char Sem);
extern void BSemOff(char Sem);
extern void BSemSw(char Sem);
extern char BSemIs(char Sem);

// вставлять в main
extern void ProcessMessages(void);


