/*
$Id: Main.c 201 2012-08-09 14:15:36Z Vlad $
 */

#include    "Main.h"

#include	"font13.h"
#include	"font23.h"
#include	"cour13.h"


//#define SHOW_PACKETS  
/***GLOBAL VAR*****************************************************************/

volatile int sys_tick = 0;
extern unsigned char ow_bit;


/******************************************************************************************/
// Ввікнено зовнішнє
const char mess1_str[] = {0xC1, 0xE1, 0xB2, 0xEB, 0xE9, 0xEC, 0xE4, 0xEC, 0xED, 0x20, 0xE6, 0xED, 0xE1, 0xEC, 0xB2, 0xF7, 0xEC, 0xB9, 0};
// Ввімкнено автономне
const char mess2_str[] = {0xC1, 0xE1, 0xB2, 0xEB, 0xE9, 0xEC, 0xE4, 0xEC, 0xED, 0x20, 0xDF, 0xE1, 0xF1, 0xED, 0xEC, 0xED, 0xEB, 0xEC, 0xE4, 0};
// джерело живлення
const char mess3_str[] = {0xE3, 0xE5, 0xE4, 0xEF, 0xE4, 0xEA, 0xED, 0x20, 0xE5, 0xE7, 0xE1, 0xEA, 0xE4, 0xEC, 0xEC, 0xFE, ' ', '2', '2', '0', 'B', 0};

const char mess_osn_str[] = {0xED, 0xF0, 0xEC, 0xED, 0xE1, 0xEC, 0xDF, 0};
const char mess_dod_str[] = {0xE3, 0xED, 0xE3, 0xDF, 0xF1, 0xE9, 0xED, 0xE1, 0xDF, 0};

const char mess_batt_off_str[] = {32, 34, 33, 34, 32, 0};


/** LOCAL PROTOTYPES **********************************************************/
void tsk_process_btn(void);
void tsk_display(void);
void tsk_ds18b20(void);
void tsk_rx(void);
void tsk_btn(void);
void tsk_int(void);

/****************************************************************************/
//EEPROM_INIT;

/****************************************************************************/
volatile char a, b;
volatile int c, d;
volatile DWORD e, f;
volatile double k, l;

//volatile BYTE packet[10];

status_t stat;

double f_T_salona = 20.0;

char str[40];
char out_packet[100];

char in_packet_int[100];

//Надписи
Label lb_napr1; // напруга осн
Label lb_napr2; // напруга дод
Label lb_temp; // температура
Label lb_mess1, lb_mess2;
Label lb_cond, lb_vent;
Label lb_fuses;

Label lb_paket_out, lb_paket_in;

// переменные, для сохранения ручного нажатия
BYTE g_fix_rear_0 = 0;
BYTE g_fix_rear_1 = 0;
BYTE g_fix_side_0 = 0;
BYTE g_fix_side_1 = 0;
BYTE g_manual_lt = 0; // признак включения света в салоне вручную
BYTE g_prev_ext_220 = 10;

BYTE g_rx = 0;

void main(void) {

    Nop();
    InitMessages();
    InitializeSystem();

    ks0108Init();

    //ks0108Fill_xy(0, 0, 0, 127, 63);

    // создаем Надписи
    LabelCreate(&lb_napr1, 12, 27, gost12, 5); // напряжение осн
    LabelCreate(&lb_napr2, 69, 27, gost12, 5); // напряжение доп


    LabelCreate(&lb_temp, 36, 52, font13, 8); // температура

    LabelCreate(&lb_mess1, 5, 0, gost12, 20); // сообщение о напряжении
    LabelCreate(&lb_mess2, 5, 9, gost12, 22);

    LabelCreate(&lb_cond, 85, 51, font13, 3);
    LabelCreate(&lb_vent, 5, 51, font13, 2);

    LabelCreate(&lb_paket_out, 0, 9, gost12, 22); // пакет
    LabelCreate(&lb_paket_in, 0, 0, gost12, 22); // пакет

    // изображения батарей
    ks0108GotoXY(8, 20);
    ks0108PutChar(32, &batt, 1);

    ks0108GotoXY(65, 20);
    ks0108PutChar(32, &batt, 1);

    // надписи батарей
    ks0108GotoXY(3, 39);
    sprintf(str, "%s", mess_osn_str);
    ks0108PutString(str, gost12, 1);

    ks0108GotoXY(60, 39);
    sprintf(str, "%s", mess_dod_str);
    ks0108PutString(str, gost12, 1);
    
    stat.st_k_back_prev = 1;
    stat.st_k_side_prev = 1;

    stat.st_k_back = 1;
    stat.st_k_side = 1;  
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
        tsk_int();

        Nop();
        Nop();


        if ((FERR) || (OERR)) {
            CREN = 0;
            NOP();
            CREN = 1;
        }
        TX_EN = 0;

        if (SWDTEN == 0) {
            SWDTEN = 1;
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
    static BYTE b_long = 0;

    // Decode all controls.
    MTouchDecode();


    // зад свет
    if (BTN_PRESSED(KEY_7_LT_OUT_BACK)) {
        Beep(10);
        stat.st_lt_back ^= 1;
        if (stat.st_lt_back == 1) g_fix_rear_1 = 1;
        else g_fix_rear_0 = 1;
        //LED4_LT_BACK = stat.st_lt_back;
        SendMessage(MSG_MANUAL_BACK);
    }

    //Бок свет
    if (BTN_PRESSED(KEY_8_LT_OUT_SIDE)) {
        Beep(10);
        stat.st_lt_side ^= 1;
        //LED5_LT_SIDE = stat.st_lt_side;
        if (stat.st_lt_side == 1) g_fix_side_1 = 1;
        else g_fix_side_0 = 1;
        SendMessage(MSG_MANUAL_SIDE);
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
#ifdef USE_POLKI       
        if (stat.st_kond < 1) {
#else
        if (stat.st_kond < 3) {
#endif

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
void batt_on_off(char status) {
    if (status == 0) {
        ks0108GotoXY(44, 23);
        ks0108PutChar(32, &sw_on, 0);
        ks0108GotoXY(44, 23);
        ks0108PutChar(32, &sw_off, 1);

    } else {
        ks0108GotoXY(44, 23);
        ks0108PutChar(32, &sw_off, 0);
        ks0108GotoXY(44, 23);
        ks0108PutChar(32, &sw_on, 1);
    }
}

/******************************************************************************************/
void tsk_display(void) {
    static double f_prev_T = -100.0;
    //double U1, U2;
    static double prev_u1 = 99.9, prev_u2 = 99.9;
    static BYTE blink, prev_cond = 10, prev_fan = 10, fuse_prev = 10;
    static BYTE prev_lt_blue = 0, prev_lt_salon = 0;
    static BYTE acc_st, charge_st = 10, prev_ext_220 = 10, prev_int_220 = 10;
    char str[30], strt[6];

    OS_TaskInit();

    LED4_LT_BACK = stat.st_lt_back;
    LED5_LT_SIDE = stat.st_lt_side;



    if ((stat.st_lt_salon != prev_lt_salon) || (stat.st_lt_blue != prev_lt_blue)) {
        prev_lt_salon = stat.st_lt_salon;
        prev_lt_blue = stat.st_lt_blue;

        ks0108GotoXY(100, 23);

        if (stat.st_lt_blue == 1) {// синий	
            // стираем норм свет
            ks0108PutChar(0x89, &font13, 0);

            ks0108GotoXY(100, 23);
            ks0108PutChar(0x8c, &font13, 1);
            ks0108PutChar(0x8c, &font13, 1);

        } else { // обычный
            // стираем синий
            ks0108PutChar(0x8c, &font13, 0);
            ks0108PutChar(0x8c, &font13, 0);

            ks0108GotoXY(100, 23);
            if (stat.st_lt_salon) {
                ks0108PutChar(0x89, &font13, 1);
            } else {
                ks0108PutChar(0x89, &font13, 0);
            }

        }
        //OS_Reset();
        //return;
    }



    if (stat.batt_k != charge_st) {
        charge_st = stat.batt_k;
        if (stat.batt_k == 1) {
            batt_on_off(1);
        } else {
            batt_on_off(0);
        };
    }

#ifndef USE_POLKI

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
#endif


    // состояние вентиляции
    if (stat.st_fan != prev_fan) {
        prev_fan = stat.st_fan;
        if (stat.st_fan == 1) {
            sprintf(str, "%c%c", 0x85, 0x86);
        } else if (stat.st_fan == 2) {
            sprintf(str, "%c%c", 0x85, 0x87);
        } else {
            sprintf(str, "  ");
        }
        Nop();
        Nop();

        LabelSetTxt(&lb_vent, str);
    }

    // пердохранитель
    if (stat.st_fuse != fuse_prev) {
        fuse_prev = stat.st_fuse;
        ks0108GotoXY(43, 37);
        if (stat.st_fuse == 1) {
            ks0108PutChar(0x8a, &font13, 1);
        } else {
            ks0108PutChar(0x8a, &font13, 0);
        }
    }


    // внешний 220
    if (stat.HV_k != prev_ext_220) {
        prev_ext_220 = stat.HV_k;
        if (stat.HV_k == 1) {
            sprintf(str, mess1_str);
            LabelSetTxt(&lb_mess1, str);
            sprintf(str, mess3_str);
            LabelSetTxt(&lb_mess2, str);

            ks0108GotoXY(115, 0);
            ks0108PutChar(0x5e, &font23, 1);
        } else {
            LabelClrTxt(&lb_mess1);
            LabelClrTxt(&lb_mess2);

            ks0108GotoXY(115, 0);
            ks0108PutChar(0x5e, &font23, 0);
        }
    } else if (stat.st_220V != prev_int_220) {
        // автономный 220
        prev_int_220 == stat.st_220V;

    }

    OS_TaskStart();
    // обновление показаний 500мс
    OS_Delay(500);
    blink++;

    //ks0108GotoXY(120, 58);
    if (g_rx) {
        //ks0108PutChar('.', &font13, 1);
        g_rx = 0;
    } else {
        //ks0108PutChar('.', &font13, 0);        
    }


    if (f_prev_T != f_T_salona) {
        f_prev_T = f_T_salona;
        if (f_T_salona != 0.0) {
            sprintf(str, "%2.1f%cC", f_T_salona, 0x8b);
        } else {
            sprintf(str, "     ");
        }
        LabelSetTxt(&lb_temp, str);
    }

    if (stat.U1 != prev_u1) {
        print_a_b(strt, stat.U1, 1, 1);
        sprintf(str, "%sB", strt);
        LabelSetTxt(&lb_napr1, str);
        prev_u1 = stat.U1;
    }

    if (stat.U2 != prev_u2) {
        print_a_b(strt, stat.U2, 1, 1);
        sprintf(str, "%sB", strt);
        LabelSetTxt(&lb_napr2, str);
        prev_u2 = stat.U2;
    }



    OS_Yield();


#ifdef SHOW_PACKETS
    OS_Yield();

    sprintf(str, "L%1u H%1u S%1u R%1u N%1u L%2u B%1u F%1u C%1u",
            stat.st_12V, stat.st_220V, stat.st_lt_side, stat.st_lt_back, stat.st_lt_noshy,
            stat.st_lt_salon, stat.st_lt_blue, stat.st_fan, stat.st_kond);
    LabelSetTxt(&lb_paket_out, str);

    OS_Yield();

    sprintf(str, "%2.1f %2.1f C%1u H%1u S%1u R%1u F%1u",
            stat.U1, stat.U2, stat.batt_k, stat.HV_k, stat.st_k_side,
            stat.st_k_back, stat.st_fuse);
    LabelSetTxt(&lb_paket_in, str);
#endif


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


#define FUSES_POS 50
#define BAT_CON_POS 26
#define HV220V 32
#define SIDE_K 38
#define BACK_K 44

/******************************************************************************************/
void tsk_rx(void) {
    if (GetMessage(MES_RX)) {
        Nop();
        Nop();
        g_rx = 1;

        if (in_packet_int[1] == 'B') {
            // зад свет
            if (in_packet_int[8] == '1') {
                Beep(10);
                stat.st_lt_back ^= 1;
                if (stat.st_lt_back == 1) g_fix_rear_1 = 1;
                else g_fix_rear_0 = 1;
                //LED4_LT_BACK = stat.st_lt_back;
                //SendMessage(MSG_MANUAL_BACK);
            }

            //Бок свет
            if (in_packet_int[13] == '1') {
                Beep(10);
                stat.st_lt_side ^= 1;
                //LED5_LT_SIDE = stat.st_lt_side;
                if (stat.st_lt_side == 1) g_fix_side_1 = 1;
                else g_fix_side_0 = 1;
                //SendMessage(MSG_MANUAL_SIDE);
            }

            return;
        }

        sprintf(out_packet, ">Com@12V=%01u 220V=%01u LRi=%01u LRe=%01u LNo=%01u Led=%02u Blu=%01u Fan=%01u Con=%01u$\r",
                stat.st_12V, stat.st_220V, stat.st_lt_side, stat.st_lt_back, stat.st_lt_noshy,
                stat.st_lt_salon, stat.st_lt_blue, stat.st_fan, stat.st_kond);

        // ответ
        putstrc(out_packet);

        // расшифровка
        sprintf(str, "%c%c.%c", in_packet_int[9], in_packet_int[10], in_packet_int[12]);
        stat.U1 = atof(str);
        sprintf(str, "%c%c.%c", in_packet_int[17], in_packet_int[18], in_packet_int[20]);
        stat.U2 = atof(str);


        if (in_packet_int[BAT_CON_POS] == '1') {
            stat.batt_k = 1;
        } else {
            stat.batt_k = 0;
        }
        if (in_packet_int[HV220V] == '0') {
            stat.HV_k = 1;
        } else {
            stat.HV_k = 0;
        }
        if (in_packet_int[SIDE_K] == '1') {
            stat.st_k_side = 1;
        } else if (in_packet_int[SIDE_K] == '0'){
            stat.st_k_side = 0;
        }
        if (in_packet_int[BACK_K] == '1') {
            stat.st_k_back = 1;
        } else if (in_packet_int[BACK_K] == '0'){
            stat.st_k_back = 0;
        }
        if (in_packet_int[FUSES_POS] == '1') {
            stat.st_fuse = 1;
        } else {
            stat.st_fuse = 0;
        }
    }



}


/******************************************************************************************/
#define CHK_REAR_0	(stat.st_k_back == 0)
#define CHK_REAR_1	(stat.st_k_back == 1)
#define CHK_SIDE_0	(stat.st_k_side == 0)
#define CHK_SIDE_1	(stat.st_k_side == 1)

/******************************************************************************************/
// концевики и внешний 220В
#define INT_WAIT_TIME 2ul // задержка реакции концевиков

void tsk_int(void) {
    static unsigned int int_rear_0 = 0;
    static unsigned int int_rear = 0;
    static unsigned int int_side = 0;
    static unsigned int wait_int_side_1 = 0;
    static BYTE prev_int_220 = 100;
    
    if(stat.st_k_back != stat.st_k_back_prev){
        stat.st_k_back_prev = stat.st_k_back;
        if(stat.st_k_back == 0){
            stat.st_lt_salon = 1;
            g_manual_lt = 0;
            stat.st_lt_back = 1;
        } else {            
            stat.st_lt_back = 0;
            if((stat.st_k_side == 1) && (g_manual_lt == 0))stat.st_lt_salon = 0;
        }
    }
    
    if(stat.st_k_side != stat.st_k_side_prev){
        stat.st_k_side_prev = stat.st_k_side;
        if(stat.st_k_side == 0){
            stat.st_lt_salon = 1;
            g_manual_lt = 0;
            stat.st_lt_side = 1;
        } else {
            stat.st_lt_side = 0;
            if((stat.st_k_back == 1) && (g_manual_lt == 0))stat.st_lt_salon = 0;
        }
    }
    
    //if((CHK_REAR_0 || CHK_SIDE_0) && (g_manual_lt == 0)) stat.st_lt_salon = 1;
    //if((CHK_REAR_1 && CHK_SIDE_1) && (g_manual_lt == 0)) stat.st_lt_salon = 0;
    
    
    
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

        if (stat.st_lt_blue > 0) {
            Beep(10);
            stat.st_lt_blue = 0;
        } else if (stat.st_lt_salon == 0) {
            Beep(10);
            stat.st_lt_salon = 31;     
            g_manual_lt = 1;
        }
    }


//    if (CHK_REAR_0) {
//        g_fix_rear_1 = 0;
//
//        if (stat.st_lt_back == 0) {
//            if ((g_fix_rear_0 == 0)/*&&(++wait_int_rear_0 > INT_WAIT_TIME)*/) {
//                stat.st_lt_back = 1;
//                ///lt(LT_PK); // вкл свет в салоне	
//                stat.st_lt_salon = 1;
//            }
//
//        } else {
//            wait_int_rear_0 = 0;
//        }
//    }
//
//    if (CHK_REAR_1) {
//        g_fix_rear_0 = 0;
//
//        if (stat.st_lt_back == 1) {
//
//            if ((g_fix_rear_1 == 0)/*&&(++wait_int_rear_1 > INT_WAIT_TIME))*/) {
//                stat.st_lt_back = 0;
//                ///if((g_manual_lt == 0)) lt(LT_MK); // выкл свет в салоне
//                if(g_manual_lt == 0) stat.st_lt_salon = 0;
//
//            }
//
//        } else {
//            wait_int_rear_1 = 0;
//        }
//    }
//
//
//    if (CHK_SIDE_0) {
//        g_fix_side_1 = 0;
//
//        if (stat.st_lt_side == 0) {
//            if ((g_fix_side_0 == 0)/*&&(++wait_int_side_0 > INT_WAIT_TIME)*/) {
//                stat.st_lt_side = 1;
//                ///lt(LT_PK); // вкл свет в салоне	
//                stat.st_lt_salon = 1;
//            }
//
//        } else {
//            wait_int_side_0 = 0;
//        }
//    }
//
//    if (CHK_SIDE_1) {
//        g_fix_side_0 = 0;
//
//        if (stat.st_lt_side == 1) {
//            if ((g_fix_side_1 == 0)/*&&(++wait_int_side_1 > INT_WAIT_TIME)*/) {
//                stat.st_lt_side = 0;
//                ///if((g_manual_lt == 0)) lt(LT_MK); // выкл свет в салоне
//                if(g_manual_lt == 0) stat.st_lt_salon = 0;
//            }
//        } else {
//            wait_int_side_1 = 0;
//
//        }
//    }



}

/******************************************************************************************/
void tsk_btn(void) {
    static int prev_tick;

    if (prev_tick == sys_tick) return;
    prev_tick = sys_tick;

    //MTouchAcquisition();
}