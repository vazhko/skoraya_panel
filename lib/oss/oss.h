#define _MSG_NO_MSG  (0)

// for mcc declarations serarately
#define TSK_INIT() \
	static int _prev_tick; \
	static int chk_count; \
	static char _yield_flag;\
	static unsigned int _state = 0; \
	static char _running_msg = _MSG_NO_MSG; \
	static char _interrupt_state; \
	Nop()
	
#define TSK_BEGIN() \
	_yield_flag = 1;\
	switch(_state){ default: case 0: \
	for(;;){ \
	Nop()	

#define TSK_ENTER() \
	static int _prev_tick; \
	static int chk_count; \
	static char _yield_flag;\
	static unsigned int _state = 0; \
	static char _running_msg = _MSG_NO_MSG; \
	static char _interrupt_state; \
	_yield_flag = 1;\
	switch(_state){ default: case 0: \
	for(;;){ \
	Nop()


#define TSK_EXIT() }}


#define TSK_DI() if(INTCONbits.GIE==1) _interrupt_state = 1; else _interrupt_state = 0; INTCONbits.GIE=0
#define TSK_EI() if(_interrupt_state==1) INTCONbits.GIE=1



#define LC_SET(st) st = __LINE__; case __LINE__:

#define TSK_WAIT_UNTIL(x) \
    do { \
        LC_SET(_state) \
	        if(!(x)) { \
            return ; \
        } \
    } while(0); \
    Nop()
    
 
#define TSK_WAIT_TC(x, tick, count) 	\
    chk_count = 0;\
    do {\
	    OS_Delay(tick);\
        if(x) {\
			break;\
        }\
    } while(++ chk_count < (count));\
    Nop() 
    
#define TSK_WAIT_TC_ST(x, tick, count, status) 	\
    chk_count = 0;\
    status = 0;\
    do {\
	    OS_Delay(tick);\
        if(x) {\
	        status = 1;\
			break;\
        }\
    } while(++ chk_count < (count));\
    Nop()    
    
 

#define TSK_WAIT_DELAY(x) \
    do { \
        TSK_DI();\
	    _prev_tick = _sys_tick; \
        LC_SET(_state) \
        if(((_sys_tick - _prev_tick) < x)) { \
	        TSK_EI();\
            return; \
        } else { \
            _prev_tick = _sys_tick; \
            TSK_EI();\
        } \
    } while(0); \
    Nop()


#define TSK_YIELD() \
    do { \
        _yield_flag = 0; \
        LC_SET(_state) \
        if(_yield_flag == 0) { \
            return; \
        } \
    } while(0); \
    Nop()
   


#define TSK_RESET()  _state = 0; return


extern volatile int _sys_tick;



#define OS_Wait(y) TSK_WAIT_UNTIL(y)
#define OS_WaitTC(x, t, c) TSK_WAIT_TC(x, t, c)
#define OS_WaitTC_ST  TSK_WAIT_TC_ST
#define OS_Yield() TSK_YIELD()
#define OS_Delay(x) TSK_WAIT_DELAY(x)
#define OS_LongDelay(t, c) TSK_WAIT_TC(0, t, c)
#define OS_Reset() TSK_RESET()

// ƒве процедуры раздел€ют OS_TaskBegin(), чтобы декларации были отдельно в начале задачи
#define OS_TaskInit() TSK_INIT()
#define OS_TaskStart() TSK_BEGIN()


#define OS_TaskBegin() TSK_ENTER()
#define OS_TaskSwitch() TSK_ENTER()
#define OS_TaskEnd() TSK_EXIT()

#define OS_Tick() _sys_tick++

#define OS_di() TSK_DI()
#define OS_ei() TSK_EI()


void OS_Shed(void);
void OS_Task_Create(void (*Task)(void));

#define TASK_COUNT 2


/*
OS_TaskBegin();

OS_Yield();
OS_TaskEnd();
*/
