/*
$Id: Main.c 201 2012-08-09 14:15:36Z Vlad $
 */

#include    "Main.h"

#include	"font13.h"
#include	"font23.h"
#include	"cour13.h"


/***GLOBAL VAR*****************************************************************/

volatile int sys_tick = 0;
extern unsigned char ow_bit;

/** LOCAL PROTOTYPES **********************************************************/
void tsk_process(void);
void tsk_display(void);
void tsk_ds18b20(void);

/****************************************************************************/
EEPROM_INIT;

/****************************************************************************/
volatile char a, b;
volatile int c, d;
volatile DWORD e, f;
volatile double k, l;

double f_T_salona;

char str[20];

//Надписи
Label lb_temp; // напруга
Label lb_temp2; // напруга

void main(void) {

    Nop();
    InitMessages();
    InitializeSystem();

    ks0108Init();

    // создаем транспарант температуры

    LabelCreate(&lb_temp, 60, 22, font13, 10); // температура
    //LabelCreate(&lb_temp2, 0, 10, font13, 10); // просто надпись
    
    //sprintf(str, "AB");
    //LabelSetTxt(&lb_temp, str);
    
    ks0108GotoXY(40, 50);
	ks0108PutChar('A', &font13, 1);
    ks0108GotoXY(50, 50);
    ks0108PutChar('B', &font13, 1);
    
    //ks0108Line(10,10, 50, 50);    
    //ks0108Line(2,5, 50, 10);    
    //ks0108Rect(60, 35, 15, 15);    
    //ks0108Line( 80, 1, 80, 60);


    Nop();
    Nop();


    INTCONbits.GIE = 1;

    Beep(10);
    
    


    while(1) {


        tsk_display();
        tsk_process();
        tsk_ds18b20();


        //Beep(10);
        Nop();
        Nop();


        if((FERR) || (OERR)) {
            CREN = 0;
            NOP();
            CREN = 1;
        }

        if(SWDTEN == 0) {
            //SWDTEN = 1;
        }
        ClrWdt();

        // обработчик state machine
        ProcessMessages();

    }
}

/******************************************************************************************/
void tsk_process(void) {
    static int prev_tick, i;


    // Decode all controls.
    MTouchDecode();


    if(BTN_PRESSED(KEY_7_LT_OUT_BACK)) {
        Beep(10);
        LED4_LT_BACK ^= 1;
    }

    if(BTN_PRESSED(KEY_8_LT_OUT_SIDE)) {
        Beep(10);
        LED5_LT_SIDE ^= 1;
    }

    if(BTN_PRESSED(KEY_6_ROZ_ON)) {
        Beep(10);
        LED2_ROZ ^= 1;
    }

    if(BTN_PRESSED(KEY_0_AVT_PWR_220)) {
        Beep(10);
        LED1_AVT_PWR ^= 1;
    }



}

/******************************************************************************************/
void tsk_display(void) {
    static double f_prev_T = -100.0;


    if(f_prev_T != f_T_salona) {
        f_prev_T = f_T_salona;
        sprintf(str, "T=%2.1f", f_T_salona);
        LabelSetTxt(&lb_temp, str);
    }


}

/******************************************************************************************/
void tsk_ds18b20(void) {
    static double f_temp;    
    static BYTE ow_err[1];

    OS_TaskBegin();

    ow_bit = 1;
    ds18b20_start();

    OS_Delay(750);
    
    //LED4_ON ^= 1;

    f_temp = ds18b20_read();

    Nop();
    Nop();

    if((f_temp != OW_ERROR) && (f_temp != 85.0)) {

        TSK_DI();
        f_T_salona = f_temp;
        TSK_EI();

        ow_err[0] = 0;
    } else {
        ow_err[0]++;
        if(ow_err[0] > 4) {
            ow_err[0] = 0;
            TSK_DI();
            f_T_salona = 0.0;
            TSK_EI();
        }
    }
    //OS_Yield();


    OS_Delay(100);
    OS_TaskEnd();
}

