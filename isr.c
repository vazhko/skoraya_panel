/*
$Id: isr.c 202 2012-08-09 14:17:18Z Vlad $
 */

#include	"Main.h"




extern volatile int sys_tick;
extern LAB_STATES curr_state;
volatile WORD beep_timer;
volatile BYTE beep_count;
volatile char dummy;
char RxTxBuff[128];
volatile char RxBuff[256];
volatile WORD j_rx = 0;
/****************************************************************************/
extern void Beep(BYTE);
BYTE recieve_from_usb(BYTE count);


/******************************************************************************************/
#define WR_BUF(adr, var) write_buff(adr, (char *)&var, sizeof(var))

void write_buff(unsigned int adr, char *var, unsigned char byte) {
    while(byte--) {
        RxTxBuff[adr + byte] = (unsigned char) (*(((unsigned char *) var) + byte));
    }
}

/****************************************************************************/
void interrupt high_priority HI_ISR(void) {
    static WORD w_pwm;
    static BYTE beep;

    // 1mS
    if((PIE1bits.TMR2IE) && (PIR1bits.TMR2IF)) {

        sys_tick++;
        OS_Tick();
        MTouchAcquisition();

        TMR2 = 0;
        PIR1bits.TMR2IF = 0;
    }

    if((PIE5bits.TMR4IE) && (PIR5bits.TMR4IF)) {

        if(beep_timer > 0) {
            beep_timer--;
            BUZ_ON = 1;
            if((beep++) & 1) {
                BUZ_ON = 0;
            } else {
                BUZ_ON = 1;
            }
            Nop();
            Nop();
        } else {
            BUZ_ON = 0;
        }


        TMR4 = 0;
        PIR5bits.TMR4IF = 0;
    }


}

/****************************************************************************/

#define GET_MORE_DATA(n)\
for(i = 2; i < ((n) + 4); i ++) {\
RxTxBuff[i] = timed_getc();\
if((FERR) || (OERR) || timeout_error)	 goto error;\
}\
if(crc_mb_check(RxTxBuff, (n) + 2) == 0) break


#define SEND_DATA(n)\
crc_mb(RxTxBuff, (n+2));\
 for(i = 0; i < ((n) + 4); i ++) {\
putbyte(RxTxBuff[i]);\
}

/****************************************************************************/
void interrupt low_priority LO_ISR(void) {
    //static BYTE beep;
    char i;
    char lcData, lcAdr;




    NOP();

    if(RCIF && RCIE) {

        //Принимаем
        // адрес
        RxTxBuff[0] = timed_getc();

        // сброс от bootloader
        if(RxTxBuff[0] == 0xEA) {
            RESET();
        }

        if((RxTxBuff[0] != RS232_ADR)) goto error;
        if((FERR) || (OERR) || timeout_error) goto error;

        RxTxBuff[1] = timed_getc();
        if((FERR) || (OERR) || timeout_error) goto error;

        // обрабатываем комманду
        switch(RxTxBuff[1]) {
            default:
                break;


            case 0x08:
                // get adress
                RxTxBuff[2] = timed_getc();
                if((FERR) || (OERR) || timeout_error) goto error;
                // get count
                RxTxBuff[3] = timed_getc();
                if((FERR) || (OERR) || timeout_error) goto error;
                // принимаем заданное кол байт
                for(i = 0; i < RxTxBuff[3] + 2; i++) {
                    RxTxBuff[i + 4] = timed_getc();
                    if((FERR) || (OERR) || timeout_error) goto error;
                }
                if(crc_mb_check(RxTxBuff, RxTxBuff[3] + 4) == 0) break;

                for(i = 0; i < RxTxBuff[3]; i++) {
                    EEPROM_WRITE(RxTxBuff[2]++, RxTxBuff[4 + i]);
                }
                SEND_DATA(0);

                break;

            case 0x09:
                GET_MORE_DATA(2);
                lcAdr = RxTxBuff[2];
                lcData = RxTxBuff[3];
                for(i = 0; i < lcData; i++) {
                    RxTxBuff[2 + i] = EEPROM_READ(lcAdr++);
                }
                SEND_DATA(lcData);
                break;

            case 0x0a:
                GET_MORE_DATA(1);
                SendMessage(USB_FLASH_SEND_CMD);

                SEND_DATA(0);
                break;


            case 0x0b:
                GET_MORE_DATA(0);
                set_ee_default();
                SEND_DATA(0);
                break;


            case 0x10:
                GET_MORE_DATA(1);
                switch(RxTxBuff[2]) {
                    default:
                        break;
                    case 1:
                        BSemOn(SEM_RX_DONE);
                        break;
                    case 2:
                        BSemOn(SEM_WR_DONE);
                        break;
                    case 3:
                        BSemOn(SEM_WR_ERR);
                        break;

                }


                Nop();
                Nop();
                break;

        }


error:
        if(RCIF) dummy = RCREG;
        if(RCIF) dummy = RCREG;
        CREN = 0;
        NOP();
        CREN = 1;
    }

}

/******************************************************************************************/
void Beep(BYTE ms10) {

    BUZ_ON_TRIS = 0;

#ifdef SOUND_ON
    //BUZ_ON = 1;
    beep_timer = ms10 * 40;
    beep_count = 1;
#else
    BUZ_ON = 0;
    beep_timer = 0;
#endif

}

