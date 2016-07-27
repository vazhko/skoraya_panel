/*
$Id: Main.c 201 2012-08-09 14:15:36Z Vlad $
 */

#include    "Main.h"

#include	"font13.h"
#include	"font23.h"
#include	"cour13.h"


/***GLOBAL VAR*****************************************************************/

LAB_STATES curr_state = ST_INIT;
volatile int sys_tick = 0;
extern unsigned char ow_bit;

/** LOCAL PROTOTYPES **********************************************************/
void tsk_process(void);
void tsk_exp(void);
void tsk_display(void);
void tsk_adc(void);
void tsk_ds18b20(void);
void tsk_pid(void);
void tsk_sign_flash(void);
void tsk_write_flash(void);
/****************************************************************************/
EEPROM_INIT;

/****************************************************************************/
volatile char a, b;
volatile int c, d;
volatile DWORD e, f;
volatile double k, l;

char str[20];

//Надписи
Label lb_napr; // напруга

void main(void) {

    Nop();
    InitMessages();
    InitializeSystem();


    ks0108Init();
    LabelCreate(&lb_napr, 0, 30, font13, 10); // напряжение
    sprintf(str, "URA!!!!");
    LabelSetTxt(&lb_napr, str);
    
    

	ks0108GotoXY(40, 50);
	ks0108PutChar(0x89, &font13, 1);
    
    ks0108GotoXY(34, 50);
	ks0108PutChar(0x8a, &font13, 1);


    Nop();
    Nop();


    INTCONbits.GIE = 1;

    Beep(10);

    while(1) {



        tsk_display();
        tsk_process();
        //tsk_ds18b20();       


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

}

/******************************************************************************************/
void tsk_display(void) {


}



/******************************************************************************************/
/*
void tsk_ds18b20(void) {
    static double f_temp;
    double f_A, f_B;
    static BYTE ow_err[2];

    OS_TaskBegin();

    ow_bit = 2;
    ds18b20_start();
    OS_Yield();
    ow_bit = 3;
    ds18b20_start();
    OS_Delay(750);

    ow_bit = 3;
    f_temp = ds18b20_read();
    if((f_temp != OW_ERROR) && (f_temp != 85.0)) {
        filter_processor(&f_temp, CH_FRIDGE);

        TSK_DI();
        g_data.f_T_fridge0 = f_temp;
        TSK_EI();

        EE_TO_RAM(PF_AC, f_A);
        EE_TO_RAM(PF_BC, f_B);
        f_temp = f_temp * f_B + f_A;

        TSK_DI();
        g_data.f_T_fridge = f_temp;
        TSK_EI();

        ow_err[0] = 0;
    } else {
        ow_err[0]++;
        if(ow_err[0] > 4) {
            ow_err[1] = 0;
            TSK_DI();
            g_data.f_T_fridge = 0.0;
            g_data.f_T_fridge0 = 0.0;
            TSK_EI();
        }
    }
    //OS_Yield();

    ow_bit = 2;
    f_temp = ds18b20_read();
    if((f_temp != OW_ERROR) && (f_temp != 85.0)) {
        filter_processor(&f_temp, CH_HEAT);
        g_data.f_T_heat = f_temp;
        ow_err[1] = 0;
    } else {
        ow_err[1]++;
        if(ow_err[1] > 4) {
            ow_err[1] = 0;
            g_data.f_T_heat = 0.0;
        }
    }


    OS_Delay(100);
    OS_TaskEnd();
}

 */