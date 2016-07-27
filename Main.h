/*
$Id: Main.h 202 2012-08-09 14:17:18Z Vlad $
*/


//13.12.2012 v0.11 - ¬ычитаю темновые засветки перед делением.



#ifndef _MAIN_H
#define _MAIN_H


#include <xc.h>

#include "GenericTypeDefs.h"
#include "mTouchConfig.h"
#include "mTouch.h"


#include "messages.h"
#include "MessagesConfig.h"
#include "TimeDelay.h"
#include "usart.h"
#include "eeprom.h"
#include "ow.h"
#include "time.h"
#include "oss.h"
#include "modbus_RTU.h"
#include "sub18.h"
#include "macro.h"
#include "ks0108.h"


/***************************************************************************/
//#define ID "TEREX TEMPEROMETR"
//#define VER "v.8.0"

#define RS232_ADR	0x55
#define SOUND_ON




/***************************************************************************/
typedef enum {
    ST_INIT = 0,
    ST_EXPERIMENT_READY,
    ST_EXPERIMENT_WAITING,
    ST_EXPERIMENT_DOING,
    ST_EXPERIMENT_DONE,
    ST_MENU,
    //ST_MENU_WR_FLASH,
    ST_VIEW_RESULTS,
    ST_VIEW_RESULTS_ASK_DEL,
    ST_DEBUG
        
} LAB_STATES;

typedef enum {
    KEY_MENU = 0,	
    KEY_START,
    KEY_M,    
    KEY_P,
    KEY_T1,
    KEY_T2,
    KEY_MM,
    KEY_PP,
    KEY_TT1,
    KEY_TT2,
    
} KEYS;


#define BTN_PRESSED(x) (MTouchGetButtonState(x) == CONTROL_PRESSED)
#define BTN_SET(x) (MTouchGetButtonState(x) = CONTROL_PRESSED)


extern void InitializeSystem(void);
extern void Beep(BYTE );
extern void pwm4_load(WORD );


#endif // _MAINDEMO_H
