/*
$Id: isr.c 202 2012-08-09 14:17:18Z Vlad $
 */

#include	"Main.h"




extern volatile int sys_tick;
volatile WORD beep_timer;
volatile BYTE beep_count;
volatile char dummy;
//char RxTxBuff[128];
volatile char RxBuff[256];
volatile WORD j_rx = 0;
/****************************************************************************/
extern void Beep(BYTE);
extern void tsk_process_btn(void);
BYTE recieve_from_usb(BYTE count);


/******************************************************************************************/
/*
#define WR_BUF(adr, var) write_buff(adr, (char *)&var, sizeof(var))
void write_buff(unsigned int adr, char *var, unsigned char byte) {
    while (byte--) {
        RxTxBuff[adr + byte] = (unsigned char) (*(((unsigned char *) var) + byte));
    }
}
 */

/****************************************************************************/
void interrupt high_priority HI_ISR(void){
    static WORD w_pwm;
    static BYTE beep;
    static WORD iSec;
    // 1mS
    if((PIE1bits.TMR2IE) && (PIR1bits.TMR2IF)){

        sys_tick ++;
        OS_Tick();
        // секундный тикер
        if(++ iSec > 1000){
            iSec = 0;
            SendMessage(MSG_SEC);
        }


        MTouchAcquisition();
        //tsk_process_btn();

        TMR2 = 0;
        PIR1bits.TMR2IF = 0;
    }

    if((PIE5bits.TMR4IE) && (PIR5bits.TMR4IF)){

        if(beep_timer > 0){
            beep_timer --;
            BUZ_ON = 1;
            if((beep ++) & 1){
                BUZ_ON = 0;
            } else {
                BUZ_ON = 1;
            }
            Nop();
            Nop();
        } else {
            BUZ_ON = 0;
            //BUZ_ON_TRIS = 1;
        }


        TMR4 = 0;
        PIR5bits.TMR4IF = 0;
    }


}

/****************************************************************************/

//52
//const char in_packet_template[] = ">PULT@V1=00.0 V2=00.0 Con=0 220=0 LRi=0 LRe=0 Fus=0 $\r";
//const char in_packet_template2[] = ">Bom@B1=1 B2=1 B3=1 B4=1$\r";

extern char out_packet[];
extern char in_packet[];

char i = 0;

/****************************************************************************/
void interrupt low_priority LO_ISR(void){
    //static BYTE beep;
    //char i;


    NOP();

    if(RCIF && RCIE){

        in_packet[i] = timed_getc();
        if((FERR) || (OERR) || timeout_error) goto error;

        // catch begin of packet
        if(in_packet[i] == '>'){
            in_packet[0] = '>';
            i = 1;
            return;
        }
        

        if((strncmp(&in_packet[0], ">PULT", 5) == 0) && (i == 51)){
            Nop();
            Nop();
            SendMessage(MES_RX);
        }
        
        if((strncmp(&in_packet[0], ">Bom@", 5) == 0) && (i == 24)){
            Nop();
            Nop();
            SendMessage(MES_RX);
            in_packet[0] = '0';
        }
/*
        if((strncmp(&in_packet[0], ">Bom@B1=1 B2=0", 14) == 0)){
            Nop();
            Nop();
            SendMessage(MES_RX);
            in_packet[0] = '0';
        }
        if((strncmp(&in_packet[0], ">Bom@B1=0 B2=1", 14) == 0)){
            Nop();
            Nop();
            SendMessage(MES_RX);
            in_packet[0] = '0';
        } 
*/

        i ++;


        return;
error:
        TX_EN = 0;
        if(RCIF) dummy = RCREG;
        if(RCIF) dummy = RCREG;
        CREN = 0;
        NOP();
        CREN = 1;
    }

}

/******************************************************************************************/
void Beep(BYTE ms10){

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

