/*
$Id: HardwareProfile.h 202 2012-08-09 14:17:18Z Vlad $
 */

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H


/******************************************************************************************/
#define GetSystemClock()		(64000000ul)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define PIC_CLK 40000000ul

#define BAUD_RATE		9600 //115200
#define USE_USART1

/******************************************************************************************/

#define BUZ_ON		LATAbits.LATA4
#define BUZ_ON_TRIS	TRISAbits.TRISA4

#define BK_ON		LATCbits.LATC4
#define BK_ON_TRIS	TRISCbits.TRISC4


#define LED1_AVT_PWR            LATBbits.LATB6
#define LED1_AVT_PWR_TRIS   	TRISBbits.TRISB6

#define LED2_ROZ            LATBbits.LATB7
#define LED2_ROZ_TRIS   	TRISBbits.TRISB7

#define LED4_LT_BACK            LATAbits.LATA6
#define LED4_LT_BACK_TRIS   	TRISAbits.TRISA6

#define LED5_LT_SIDE             LATAbits.LATA7
#define LED5_LT_SIDE_TRIS	TRISAbits.TRISA7


#define TX_EN       LATCbits.LATC5
#define TX_EN_TRIS  TRISCbits.TRISC5


//  KS0108
#define LCD_TRIS		TRISD
#define LCD_DATA		PORTD
#define LCD_LAT			LATD

#define LCD_ENABLE        LATE0
#define LCD_RW            LATE1
#define LCD_DI            LATE2

#define LCD_ENABLE_TRIS        TRISEbits.TRISE0
#define LCD_RW_TRIS            TRISEbits.TRISE1
#define LCD_DI_TRIS            TRISEbits.TRISE2

#define LCD_CS1           LATC3	
#define LCD_CS2           LATC2
#define LCD_RESET         LATC0

#define LCD_CS1_TRIS        TRISCbits.TRISC3
#define LCD_CS2_TRIS        TRISCbits.TRISC2
#define LCD_RESET_TRIS      TRISCbits.TRISC0

#define LCD_DATA_IN			PORTD		// Data Input Register
#define LCD_DATA_OUT		LATD		// Data Output Register
#define LCD_DATA_DIR		TRISD		// Data Direction Register for Data Port



#define	OW_TRIS		TRISC
#define	OW_LAT		LATC
#define	OW_PORT		PORTC
#define	OW_BIT		ow_bit

// SENSORS CONNECTIONS

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  12

#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  10		
			
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  8
					
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  5
#define CH3_AN_NUM  13
				
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  0
#define CH4_AN_NUM  0
					
#define CH5_TRIS    &TRISA
#define CH5_LAT     &LATA
#define CH5_IO_BIT  1
#define CH5_AN_NUM  1
						
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  4
#define CH6_AN_NUM  11


#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  3
#define CH7_AN_NUM  9

#define CH8_TRIS    &TRISA
#define CH8_LAT     &LATA
#define CH8_IO_BIT  2
#define CH8_AN_NUM  2

#define CH9_TRIS    &TRISA
#define CH9_LAT     &LATA
#define CH9_IO_BIT  3
#define CH9_AN_NUM  3

#define CH10_TRIS    &TRISA
#define CH10_LAT     &LATA
#define CH10_IO_BIT  5
#define CH10_AN_NUM  4





#endif  //HARDWARE_PROFILE_H
