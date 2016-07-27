/*

high level delay routines - see delay.h for more info.

Designed by Shane Tolmie of KeyGhost corporation.  Freely distributable.
Questions and comments to shane@keyghost.com
PICuWEB - Program PIC micros with C. Site has FAQ and sample source code. http://www.workingtex.com/htpic

For Microchip PIC18xxxxx and Hi-Tech C
*/

#ifndef __DELAY_C
#define __DELAY_C


#if defined(__XC)
#include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
#include <pic18.h>        /* HiTech General Include File */
#endif

#include	"delay.h"



unsigned char delayus_variable;

void DelayBigUs(unsigned int cnt) {
unsigned char	i;

	i = (unsigned char)(cnt>>8);
	while(i >= 1)
	{
		i--;
		DelayUs(253);
		NOP();
	}
	DelayUs((unsigned char)(cnt & 0xFF));
}
//////////////////////////////////////////////////////////////////////////////////
void DelayMs(unsigned char cnt)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
			NOP();
		} while(--i);
	} while(--cnt);
}

//this copy is for the interrupt function
void DelayMs_ISR(unsigned char cnt)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
		} while(--i);
	} while(--cnt);
}
/////////////////////////////////////////////////////////////////////////////////
void DelayBigMs(unsigned int cnt)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
			NOP();
		} while(--i);
	} while(--cnt);
}

void DelayS(unsigned char cnt)
{
	unsigned char i;
	do {
		i = 4;
		do {
			DelayMs(250);
			NOP();
		} while(--i);
	} while(--cnt);
}

void DelayS_ISR(unsigned char cnt)
{
	unsigned char i;
	do {
		i = 4;
		do {
			DelayMs_ISR(250);
			NOP();
		} while(--i);
	} while(--cnt);
}

#endif


