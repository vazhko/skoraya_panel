#ifndef 	USART_H
    #define 	USART_H

    #include 	"compil.h"
    #include 	"HardwareProfile.h"
    #include	"GenericTypeDefs.h"
    #include 	"macro.h" 
 
    
	// если в микроконтроллере один usart
	// то ничего не надо указывать
	// если в микроконтроллере два usart
	// HardwareProfile.h
	// #define USE_USART1
	// и / или 
	// #define USE_USART2

	// для совместимости
    #ifndef BAUD_RATE
        #define BAUD_RATE	BAUD
    #endif
    
    

    #ifndef GetSystemClock()
        #if defined FOSC
            #define GetSystemClock()	FOSC
        #elif defined CLOCK_FREQ
            #define GetSystemClock()	CLOCK_FREQ
        #else
            #error usart.h - please define GetSystemClock() correctly
        #endif
    #endif


    #define BAUD_ERROR      	4

    #if ((GetSystemClock()/(16UL * BAUD_RATE))-1UL) < 256UL
        #if defined(_18F252) || defined(_18F452)
// нет бита BRG16
            #define SPBRG_SET (GetSystemClock()/(16UL * BAUD_RATE))-1
            #define BRGH_SET 1
            #define BRG16_SET 0
            #define BAUD_REAL (GetSystemClock()/((16UL)*(SPBRG_SET+1)))
        #else
            #if ((GetSystemClock()/(16UL * BAUD_RATE))-1UL) < 32UL
                #define SPBRG_SET (GetSystemClock()/(4UL * BAUD_RATE))-1
                #define BRGH_SET 1
                #define BRG16_SET 1
                #define BAUD_REAL (GetSystemClock()/((4UL)*(SPBRG_SET+1)))
            #else
                #define SPBRG_SET (GetSystemClock()/(16UL * BAUD_RATE))-1
                #define BRGH_SET 1
                #define BRG16_SET 0
                #define BAUD_REAL (GetSystemClock()/((16UL)*(SPBRG_SET+1)))
            #endif
        #endif

    #else
        #define SPBRG_SET (GetSystemClock()/(64UL * BAUD_RATE))-1
        #define BRGH_SET 0
        #define BRG16_SET 0
        #define BAUD_REAL (GetSystemClock()/((64UL)*(SPBRG_SET+1)))
    #endif

//check that error < N% (see manual for USART baud rates for async. mode)
    #if BAUD_REAL > BAUD_RATE
        #if (((BAUD_REAL - BAUD_RATE)*100UL)/BAUD_RATE) > BAUD_ERROR
            #error  "baud rate error percentage is too high"   
        #endif
    #else
        #if (((BAUD_RATE - BAUD_REAL)*100UL)/BAUD_RATE) > BAUD_ERROR
            #error  "baud rate error percentage is too high"   
        #endif
    #endif


    #ifndef UART_DIVIDER
        #define UART_DIVIDER	SPBRG_SET
    #endif

    #ifdef USE_USART2
        #if ((GetSystemClock()/(16UL * BAUD_RATE2))-1UL) < 256UL

            #if ((GetSystemClock()/(16UL * BAUD_RATE2))-1UL) < 32UL
                #define SPBRG_SET2 (GetSystemClock()/(4UL * BAUD_RATE2))-1
                #define BRGH_SET2 1
                #define BRG16_SET2 1
                #define BAUD_REAL2 (GetSystemClock()/((4UL)*(SPBRG_SET2+1)))
            #else
                #define SPBRG_SET2 (GetSystemClock()/(16UL * BAUD_RATE2))-1
                #define BRGH_SET2 1
                #define BRG16_SET2 0
                #define BAUD_REAL2 (GetSystemClock()/((16UL)*(SPBRG_SET2+1)))
            #endif
        #else
            #define SPBRG_SET2 (GetSystemClock()/(64UL * BAUD_RATE2))-1
            #define BRGH_SET2 0
            #define BRG16_SET2 0
            #define BAUD_REAL2 (GetSystemClock()/((64UL)*(SPBRG_SET2+1)))
        #endif

//check that error < N% (see manual for USART baud rates for async. mode)
        #if BAUD_REAL2 > BAUD_RATE2
            #if (((BAUD_REAL2 - BAUD_RATE2)*100UL)/BAUD_RATE2) > BAUD_ERROR
                #error  "baud rate error 2 percentage is too high"   
            #endif
        #else
            #if (((BAUD_RATE2 - BAUD_REAL2)*100UL)/BAUD_RATE2) > BAUD_ERROR
                #error  "baud rate 2 error percentage is too high"   
            #endif
        #endif

    #endif

/***************************************************************************************/
// for using one port
    #if defined USE_USART1
        #define TRMT TRMT1
        #define BRGH BRGH1
        #define TX9 TX91
        #define RX9 RX91
        #define TXEN TXEN1
        #define CREN CREN1
        #define SPEN SPEN1
        #define BRG16 BRG161
        #define SREN SREN1
        #define SYNC SYNC1
        #define FERR FERR1
        #define OERR OERR1
    #endif

/***************************************************************************************/
    #define put_ch(n)		putbyte((n));


/***************************************************************************************/
extern void putbyte(char byte);
extern void putword(int word);
extern char getbyte(void);
extern char getbyte_(void);
extern char timed_getc(void);
extern void putstr(const char *);
extern void putstrc(char *);
extern void uart_init(void);	// для совместимости
extern void uart16_init(void);	// новая функция

extern void putbyte2(char byte);
extern void putword2(int word);
extern char getbyte2(void);
extern char timed_getc2(void);
extern void putstr2(const char *);
extern void uart16_init2(void);

/***************************************************************************************/
extern volatile BYTE timeout_error;
extern volatile BYTE timeout_error2;

#endif
