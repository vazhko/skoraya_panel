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
//#define USE_POLKI
#define VER "v.1.0"

#define RS232_ADR	0x55
#define SOUND_ON




/***************************************************************************/
/*
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
*/

typedef enum {
    KEY_0_AVT_PWR_220 = 0,    
    KEY_1_LT_NOSHY,
    KEY_2_LT_SALON_M,           
    KEY_3_LT_SALON_P,             
    KEY_4_VENT_OUT,
    KEY_5_VENT_IN,
    KEY_6_ROZ_ON,
    KEY_7_LT_OUT_BACK,           
    KEY_8_LT_OUT_SIDE,
    KEY_9_LT_KOND_M,
    KEY_10_LT_KOND_P,
            
    KEY_2_LT_SALON_MM,
    KEY_3_LT_SALON_PP,    
} KEYS;

typedef struct {
    BYTE st_12V;
    BYTE st_220V;
    BYTE st_lt_back;
    BYTE st_lt_side;
    BYTE st_lt_noshy;
    BYTE st_lt_salon;
    BYTE st_lt_blue;
    BYTE st_fan;
    BYTE st_kond;
    
    double U1;
    double U2;
    BYTE  batt_k;
    BYTE  HV_k;
    BYTE st_k_side;
    BYTE st_k_back;
    
    BYTE st_fuse;
}status_t;


#define BTN_PRESSED(x) (MTouchGetButtonState(x) == CONTROL_PRESSED)
#define BTN_RELEASED(x) (MTouchGetButtonState(x) == CONTROL_RELEASED)
#define BTN_SET(x) (MTouchGetButtonState(x) = CONTROL_PRESSED)


extern void InitializeSystem(void);
extern void Beep(BYTE );
extern void pwm4_load(WORD );


#endif // _MAINDEMO_H
