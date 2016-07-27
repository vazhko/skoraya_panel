#include	<stddef.h>
#include	"oss.h"

volatile unsigned char curr_task;
volatile int _sys_tick;

/*
void (*pTask[TASK_COUNT])(void);

void OS_Shed(void) {
    curr_task ++;
    if(curr_task >= TASK_COUNT) curr_task = 0;
    if(pTask[curr_task] != NULL) pTask[curr_task]();
}

void OS_Task_Create(void (*Task)(void)) {
    static unsigned char tsk_num;
    pTask[tsk_num] = Task;
    tsk_num ++;
}

*/