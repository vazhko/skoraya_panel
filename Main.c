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
void tsk_process_btn(void);
void tsk_display(void);
void tsk_ds18b20(void);
void tsk_rx(void);
void tsk_btn(void);

/****************************************************************************/
EEPROM_INIT;

/****************************************************************************/
volatile char a, b;
volatile int c, d;
volatile DWORD e, f;
volatile double k, l;

//volatile BYTE packet[10];

status_t stat;

double f_T_salona = 20.0;

char str[40];
char out_packet[63];
char in_packet[55];

//Надписи
Label lb_temp; // t
Label lb_paket; // paket
Label lb_cond, lb_vent;
Label lb_v1, lb_v2;

void main(void) {

    Nop();
    InitMessages();
    InitializeSystem();

    ks0108Init();

    //ks0108Fill_xy(0, 0, 0, 127, 63);

    // создаем Надписи
    LabelCreate(&lb_temp, 0, 0, font13, 10); // температура
    LabelCreate(&lb_paket, 0, 22, font13, 22); // просто надпись

    LabelCreate(&lb_cond, 85, 51, font13, 4);
    LabelCreate(&lb_vent, 5, 51, font13, 4);
    
    LabelCreate(&lb_v1, 0, 11, font13, 10);
    LabelCreate(&lb_v2, 63, 11, font13, 10);
    
    /*
        sprintf(str, "AB");
        LabelSetTxt(&lb_temp, str);

        ks0108GotoXY(20, 50);
        ks0108PutChar('A', &font13, 1);

        ks0108GotoXY(1, 40);
        ks0108PutChar('B', &font23, 1);

        ks0108GotoXY(100, 30);
        ks0108PutChar(0x5e, &font23, 1);

        ks0108GotoXY(100, 0);
        ks0108PutChar(0x89, &font13, 1);

        // линия
        ks0108Line(5, 30, 60, 60);
        // прямоугольник
        ks0108Rect(60, 35, 15, 15);
     */


    Nop();
    Nop();


    //INTCONbits.GIE = 1;

    Beep(10);




    while (1) {


        tsk_display();
        tsk_process_btn();
        tsk_ds18b20();
        tsk_rx();
        tsk_btn();

        Nop();
        Nop();


        if ((FERR) || (OERR)) {
            CREN = 0;
            NOP();
            CREN = 1;
        }
        TX_EN = 0;

        if (SWDTEN == 0) {
            //SWDTEN = 1;
        }
        ClrWdt();

        if (INTCONbits.GIE == 0) {
            INTCONbits.GIE = 1;
        }

        // обработчик state machine
        ProcessMessages();

    }
}

/******************************************************************************************/
// обработка кнопок

void tsk_process_btn(void) {
    static int prev_tick, i;


    // Decode all controls.
    MTouchDecode();


    // зад свет
    if (BTN_PRESSED(KEY_7_LT_OUT_BACK)) {
        Beep(10);
        stat.st_lt_back ^= 1;
        LED4_LT_BACK = stat.st_lt_back;
    }

    //Бок свет
    if (BTN_PRESSED(KEY_8_LT_OUT_SIDE)) {
        Beep(10);
        stat.st_lt_side ^= 1;
        LED5_LT_SIDE = stat.st_lt_side;
    }

    // 12В
    if (BTN_PRESSED(KEY_6_ROZ_ON)) {
        Beep(10);
        stat.st_12V ^= 1;
        LED2_ROZ = stat.st_12V;
    }

    //220В
    if (BTN_PRESSED(KEY_0_AVT_PWR_220)) {
        Beep(10);
        stat.st_220V ^= 1;
        LED1_AVT_PWR = stat.st_220V;
    }

    //  управление светом салона
    if (BTN_PRESSED(KEY_2_LT_SALON_M)) {
        Beep(10);
        if (stat.st_lt_salon == 0) {
            stat.st_lt_blue ^= 1;
        } else {
            stat.st_lt_salon = 0;
            stat.st_lt_blue = 0;
        }
    }

    if (BTN_PRESSED(KEY_3_LT_SALON_P)) {
        Beep(10);
        stat.st_lt_salon = 31;

        if (stat.st_lt_salon > 0) {
            stat.st_lt_blue = 0;
        }
    }
    /*
        // ступенчатое управление яркостью салона
        if (BTN_PRESSED(KEY_2_LT_SALON_MM)) {
            if (stat.st_lt_salon > 0) {
                Beep(10);
                stat.st_lt_salon--;
                stat.st_lt_blue = 0;
            }
        }

        if (BTN_PRESSED(KEY_3_LT_SALON_PP)) {
            if (stat.st_lt_salon < 31) {
                Beep(10);
                stat.st_lt_salon++;
            }
            if (stat.st_lt_salon > 0) {
                stat.st_lt_blue = 0;
            }
        }
     */

    // ноши
    if (BTN_PRESSED(KEY_1_LT_NOSHY)) {
        Beep(10);
        stat.st_lt_noshy ^= 1;
    }

    // кондиционер
    if (BTN_PRESSED(KEY_9_LT_KOND_M)) {
        if (stat.st_kond > 0) {
            Beep(10);
            stat.st_kond--;
        }
    }

    if (BTN_PRESSED(KEY_10_LT_KOND_P)) {
        if (stat.st_kond < 3) {
            Beep(10);
            stat.st_kond++;
        }
    }

    // вентиляция
    if (BTN_PRESSED(KEY_4_VENT_OUT)) {
        Beep(10);
        stat.st_fan = (stat.st_fan == 0) ? 1 : 0;
    }

    if (BTN_PRESSED(KEY_5_VENT_IN)) {
        Beep(10);
        stat.st_fan = (stat.st_fan == 0) ? 2 : 0;
    }


}

/******************************************************************************************/
void tsk_display(void) {
    static double f_prev_T = -100.0;
    static BYTE blink, prev_cond = 5, prev_fan = 10, prev_lt_blue = 0, prev_lt_salon = 0;
    char str[30], strt[6];

    OS_TaskInit();



    OS_TaskStart();
    // обновление показаний 500мс
    OS_Delay(500);

    blink++;

    if (f_prev_T != f_T_salona) {
        f_prev_T = f_T_salona;
        if (f_T_salona != 0.0) {
            //print_a_b(strt, f_T_salona, 1, 1);
            //sprintf(str, "%s%cC", strt, 0x8b);
            sprintf(str, "T=%2.1f", f_T_salona);
        } else {
            sprintf(str, "     ");
        }
        LabelSetTxt(&lb_temp, str);
    }
    
    sprintf(str, "U1=%c%c.%cB", in_packet[9], in_packet[10], in_packet[12]);
    LabelSetTxt(&lb_v1, str);
    
    sprintf(str, "U2=%c%c.%cB", in_packet[17], in_packet[18], in_packet[20]);
    LabelSetTxt(&lb_v2, str);    
    
    
    

    /*
    sprintf(str, "%01u %01u %01u %01u %01u %02u %01u %01u %01u",
            stat.st_12V, stat.st_220V, stat.st_lt_back, stat.st_lt_side, stat.st_lt_noshy,
            stat.st_lt_salon, stat.st_lt_blue, stat.st_fan, stat.st_kond);


    LabelSetTxt(&lb_paket, str);
    */ 
    
    OS_Yield();

    // пердохранитель
    /*
    ks0108GotoXY(53, 37);
    if((FUSES == 0) && ((blink)&1)){	
        ks0108PutChar(0x8a, &font13, 1);
    } else {
        ks0108PutChar(0x8a, &font13, 0);
    }
     */
    ///*

    if ((stat.st_lt_salon != prev_lt_salon) || (stat.st_lt_blue != prev_lt_blue)) {
        prev_lt_salon = stat.st_lt_salon;
        prev_lt_blue = stat.st_lt_blue;

        ks0108GotoXY(40, 50);
        ks0108PutChar(0x89, &font13, 0);

        if (stat.st_lt_salon > 0) {
            ks0108PutChar(0x89, &font13, 1);
        } else {
            ks0108PutChar(0x89, &font13, 0);
        }

        if (stat.st_lt_blue) {
            ks0108PutChar(0x89, &font13, 1);
        } else {
            ks0108PutChar(0x89, &font13, 0);
        }
    }


    //*/
    OS_Yield();


    // кондей

    if (stat.st_kond != prev_cond) {
        prev_cond = stat.st_kond;
        if (stat.st_kond == 0) {
            sprintf(str, "   ");
        } else if (stat.st_kond == 1) {
            sprintf(str, "%c  ", 0x84);
        } else if (stat.st_kond == 2) {
            sprintf(str, "%c%c ", 0x84, 0x84);
        } else if (stat.st_kond == 3) {
            sprintf(str, "%c%c%c", 0x84, 0x84, 0x84);
        }
        LabelSetTxt(&lb_cond, str);
    }


    OS_Yield();

    // состояние вентиляции
    if (stat.st_fan != prev_fan) {
        prev_fan = stat.st_fan;
        if (stat.st_fan == 1) {
            //sprintf(str, "%c", 0x85, 0x86);
            sprintf(str, "%c", 0x85);
        } else if (stat.st_fan == 2) {
            //sprintf(str, "%c", 0x85, 0x87);
            sprintf(str, "%c", 0x85);
        } else {
            sprintf(str, " ");
            //sprintf(str, "%c", ' ', ' ');
        }
        LabelSetTxt(&lb_vent, str);
    }

    OS_Yield();

    OS_TaskEnd();



}

/******************************************************************************************/
void tsk_ds18b20(void) {
    static double f_temp;
    static BYTE ow_err[1];

    OS_TaskBegin();

    ow_bit = 1;
    ds18b20_start();

    OS_Delay(750);

    f_temp = ds18b20_read();

    Nop();
    Nop();

    if ((f_temp != OW_ERROR) && (f_temp != 85.0)) {

        TSK_DI();
        f_T_salona = f_temp;
        TSK_EI();

        ow_err[0] = 0;
    } else {
        ow_err[0]++;
        if (ow_err[0] > 4) {
            ow_err[0] = 0;
            TSK_DI();
            f_T_salona = 0.0;
            TSK_EI();
        }
    }

    OS_Delay(100);
    OS_TaskEnd();
}

/******************************************************************************************/
void tsk_rx(void) {
    if (GetMessage(MES_RX)) {

        Nop();
        Nop();

        //DelayMs(5);
        //TX_EN = 0;        

        sprintf(out_packet, ">Com@12V=%01u 220V=%01u LRi=%01u LRe=%01u LNo=%01u Led=%02u Blu=%01u Fan=%01u Con=%01u$\r",
                stat.st_12V, stat.st_220V, stat.st_lt_back, stat.st_lt_side, stat.st_lt_noshy,
                stat.st_lt_salon, stat.st_lt_blue, stat.st_fan, stat.st_kond);
    }

}

/******************************************************************************************/
void tsk_btn(void) {
    static int prev_tick;

    if (prev_tick == sys_tick) return;
    prev_tick = sys_tick;

    //MTouchAcquisition();
}