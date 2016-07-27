/*
$Id: HardwareProfile.h 202 2012-08-09 14:17:18Z Vlad $
 */

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H


/******************************************************************************************/
#define GetSystemClock()		(64000000ul)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define BAUD_RATE		19200 //115200
#define USE_USART1
#define USE_OLD_MODBUS

/******************************************************************************************/



#define BUZ_ON		LATAbits.LATA4
#define BUZ_ON_TRIS	TRISAbits.TRISA4

#define BK_ON		LATCbits.LATC4
#define BK_ON_TRIS	TRISCbits.TRISC4


#define	OW_TRIS		TRISB
#define	OW_LAT		LATB
#define	OW_PORT		PORTB
#define	OW_BIT		ow_bit

// SENSORS CONNECTIONS

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0		
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
					
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
				
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
					
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4
						
#define CH5_TRIS    &TRISE
#define CH5_LAT     &LATE
#define CH5_IO_BIT  0
#define CH5_AN_NUM  5


#define	FLTR_DATA_COUNT		5		// количество каналов
#define	FLTR_ERR_COUNT		2		// количество одиночных помех
#define	FLTR_ERR_VALUE		2.0	// величина отклонения, классифицируемое, как "одиночка"



#endif  //HARDWARE_PROFILE_H
