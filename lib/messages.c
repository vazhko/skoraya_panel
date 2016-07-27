#include "messages.h"


/******************************************************************************************/
volatile char Messages[MAX_MESSAGES];

#ifdef USE_MESSAGE_PARAM
volatile char MesParam[MAX_MESSAGES];
#endif

#ifdef MAX_BROADCAST_MESSAGES
volatile char BroadcastMessages[MAX_BROADCAST_MESSAGES];
#endif

#ifdef MAX_SEMAPHORES
volatile char BSem[MAX_SEMAPHORES];
#endif

/******************************************************************************************/
void InitMessages(void) {
	char i;
	
	for(i = 0; i < (MAX_MESSAGES); i++){		
		Messages[i] = MSG_PHASE_NONE;
#ifdef USE_MESSAGE_PARAM
		MesParam[i] = 0;
#endif		
	}
		
#ifdef MAX_BROADCAST_MESSAGES		
	for(i = 0; i < MAX_BROADCAST_MESSAGES; i++)
		BroadcastMessages[i] = MSG_PHASE_NONE;
#endif
		
#ifdef MAX_SEMAPHORES		
	for(i = 0; i < MAX_SEMAPHORES; i++)
		BSem[i] = 0;
#endif

}

/******************************************************************************************/
void SendMessage(char Msg)	{
	Messages[Msg] = MSG_PHASE_PREPARE;
}

/******************************************************************************************/
char GetMessage(char Msg) {
	
	if(Messages[Msg] == MSG_PHASE_READY) {
		Messages[Msg] = MSG_PHASE_NONE;
		return 1;
	}	
	return 0;
}

/******************************************************************************************/
// проверка сообщения без сбрасывания
char ReadyMessage(char Msg) {
	
	if(Messages[Msg] == MSG_PHASE_READY) {		
		return 1;
	}	
	return 0;
}


/******************************************************************************************/
#ifdef USE_MESSAGE_PARAM

/******************************************************************************************/
char GetMessageP(char Msg, char *pParam) {
	
	if(Messages[Msg] == MSG_PHASE_READY) {
			Messages[Msg] = MSG_PHASE_NONE;
			*pParam = MesParam[Msg];
			return 1;
	}	
	return 0;
}

/******************************************************************************************/
void SendMessageP(char Msg, char Param)	{
	Messages[Msg] = MSG_PHASE_PREPARE;
	MesParam[Msg] = Param;
}

#endif


/******************************************************************************************/
/******************************************************************************************/
#ifdef MAX_BROADCAST_MESSAGES
void SendBroadcastMessage(char Msg){	
	BroadcastMessages[Msg] = MSG_PHASE_PREPARE;	
}


char GetBroadcastMessage(char Msg) {
	
	if(BroadcastMessages[Msg] == MSG_PHASE_READY){
		return 1;
	}	
	return 0;
}
#endif


/******************************************************************************************/
void ProcessMessages(void){	
	char i;	
	for(i = 0; i < (MAX_MESSAGES); i++){
		if(Messages[i] == MSG_PHASE_READY) Messages[i] = MSG_PHASE_NONE;
		if(Messages[i] == MSG_PHASE_PREPARE) Messages[i] = MSG_PHASE_READY;
	}
	
#ifdef MAX_BROADCAST_MESSAGES	
	for(i = 0; i < MAX_BROADCAST_MESSAGES; i++)	{
		if(BroadcastMessages[i] == MSG_PHASE_READY) BroadcastMessages[i] = MSG_PHASE_NONE;
		if(BroadcastMessages[i] == MSG_PHASE_PREPARE) BroadcastMessages[i] = MSG_PHASE_READY;
	}
#endif
}



/******************************************************************************************/
/******************************************************************************************/
#ifdef MAX_SEMAPHORES



char BSemIs(char Sem) {
	if(BSem[Sem] == 0){
		return 0;
	} else {
		return 1;
	}
}

/******************************************************************************************/
void BSemOn(char Sem){
	BSem[Sem] = 1;
}

/******************************************************************************************/
void BSemOff(char Sem){
	BSem[Sem] = 0;
}

/******************************************************************************************/
void BSemSw(char Sem){
	BSem[Sem] ^= 1;
}
#endif
