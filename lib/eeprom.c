#include 	"compil.h"

#ifndef _EEPROMSIZE
#define _EEPROMSIZE 256
#endif

#include 	"eeprom.h"	


/******************************************************************************************/		
void write___ee(unsigned int adr, unsigned char *var, unsigned char byte){	
while(byte--){
	EEPROM_WRITE(adr+byte, (unsigned char)(*(((unsigned char *)var)+byte)));
	};		
}

/******************************************************************************************/
void read___ee(unsigned int adr, unsigned char *var, unsigned char byte){
while(byte--){	
	(unsigned char)(*(((unsigned char *)var)+byte))=EEPROM_READ(adr+byte);
	};		
}

/******************************************************************************************/
#ifdef USE_EE_DATA
#ifdef EE_DATA
const char eedata[]= EE_DATA;
void set_ee_default(void){	
	unsigned int i;
	char c, c1;
	for(i = 0; i < 256; i ++){
		//EEPROM_WRITE(i, eedata[i]);
		c = eedata[i];
		EE_TO_RAM(i, c1);
		if(c != c1){
			RAM_TO_EE(i, c);
		}
	}	
}
#endif
#endif

/******************************************************************************************/
#ifdef IS_WRITEBLE
unsigned char is_ee_wr(unsigned int adr) {
	const unsigned int is_wr_byte[] = {IS_WRITEBLE};
	unsigned char by, bi;
	
	by = adr / 16;
	bi = adr % 16;
	
	if(((is_wr_byte[by]) & (1 <<(bi)))) return 1; else return 0;	
}
#endif


