

#include	"macro.h"
#include	"usart.h"
#ifdef USE_RS485
    #include	"delay.h"
#endif

volatile BYTE timeout_error;

/******************************************************************************************/
// дл€ совместимости со старыми верси€ми

void uart_init(void) {
#ifdef 	BRGH_VAL
    BRGH = BRGH_VAL;
#else
    BRGH = 0;
#endif
    SPBRG = UART_DIVIDER;
    TXEN = 1;
    CREN = 1;
    SPEN = 1;
}

/******************************************************************************************/
void uart16_init(void) {

    SPBRG = SPBRG_SET;
    BRGH = BRGH_SET; //data rate for sending/receiving
#if defined(_18F252) || defined(_18F452)
    // нет бита BRG16
#else
    BRG16 = BRG16_SET;
#endif

    SYNC = 0; //asynchronous
    SPEN = 1; //enable serial port pins
    CREN = 1; //enable reception
    SREN = 0; //no effect
    TXIE = 0; //disable tx interrupts
    RCIE = 0; //disable rx interrupts
    TX9 = 0; //8-bit transmission
    RX9 = 0; //8-bit reception
    TXEN = 0; //reset transmitter
    TXEN = 1; //enable the transmitter
}

/******************************************************************************************/
void putbyte(char byte) {
#ifdef USE_RS485
    if (TX_EN == 0) {
        TX_EN = 1;
        DelayMs(5);
    }
#endif
    while (!TRMT);
    TXREG = byte;
}

/******************************************************************************************/
void putword(int word) {
#ifdef USE_RS485
    if (TX_EN == 0) {
        TX_EN = 1;
        DelayMs(5);
    }
#endif
    while (!TRMT);
    TXREG = BYTE_1(word);
    TXREG = BYTE_0(word);

}

/******************************************************************************************/
char getbyte(void) {
    /* set when register is not empty */
    while (!RCIF) {
        NOP();
        continue;
    }
    return RCREG;
}


/******************************************************************************************/
char getbyte_(void) {
    /* set when register is not empty */
    if (RCIF) {
    	return RCREG;
	}  else {
		return 0;
	}
}

/******************************************************************************************/
char timed_getc(void) {
    WORD timeout;

    timeout_error = 0;
    timeout = 0;

    do NOP(); while (!RCIF && (++timeout < 10000)); // 1/2 second
    //delay_us(10);
    if (RCIF) {
        return (getbyte());
    } else {
        timeout_error = 1;
        return (0);
    }
}

/******************************************************************************************/
void putstr(const char *s) {
    while (*s) putbyte(*s++);
}

/******************************************************************************************/
void putstrc(char *s) {
    while (*s) putbyte(*s++);
}



#ifdef USE_USART2

BYTE volatile timeout_error2;

/******************************************************************************************/
void uart16_init2(void) {

    SPBRG2 = SPBRG_SET2;
    BRGH2 = BRGH_SET2; //data rate for sending/receiving

    BRG162 = BRG16_SET2;

    SYNC2 = 0; //asynchronous
    SPEN2 = 1; //enable serial port pins
    CREN2 = 1; //enable reception
    SREN2 = 0; //no effect
    TX2IE = 0; //disable tx interrupts
    RC2IE = 0; //disable rx interrupts
    TX92 = 0; //8-bit transmission
    RX92 = 0; //8-bit reception
    TXEN2 = 0; //reset transmitter
    TXEN2 = 1; //enable the transmitter
}

/******************************************************************************************/
void putbyte2(char byte) {
    while (!TRMT2);
    TXREG2 = byte;
}

/******************************************************************************************/
char getbyte2(void) {
    while (!RC2IF) {
        NOP();
        continue;
    }
    return RCREG2;
}

/******************************************************************************************/
char timed_getc2(void) {
    WORD timeout;

    timeout_error2 = 0;
    timeout = 0;

    do NOP(); while (!RC2IF && (++timeout < 10000));
    //delay_us(10);
    if (RC2IF) {
        return (getbyte2());
    } else {
        timeout_error2 = 1;
        return (0);
    }
}

/******************************************************************************************/
void putstr2(const char *s) {
    while (*s) putbyte2(*s++);
}

#endif
