#include <pic18.h>
#include "macro.h"
#include "HardwareProfile.h"
#include "ow.h"


/******************************************************************************************/
static unsigned char __gie;


#ifdef	OW_USE_CHANGING_BIT
// Для изменения в реальном времени Пина для датчика
// не делать #define	OW_BIT		6
// использовать функции ds18b20_start_port(); void ds18b20_read_bit(6);
unsigned char ow_bit;
#endif

#ifdef OW_USE_CHARGING_PORT
// Для изменения в реальном времени портов для датчика
volatile unsigned char *PTRIS, *PLAT, *PPORT;
#endif

/******************************************************************************************/
// crc lookup table
#ifndef OW_NO_USE_CHKSM

char scratch[9];

char const dscrc_table[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};
#else
char scratch[2];
#endif

/******************************************************************************************/
char init_1wire(void);
char read_1wire(void);
void write_1wire(char data);
void write_1wire_port(char data);

/******************************************************************************************/
// One wire crc
void ow_delay(unsigned int del) {
    while(del--) Nop();
}

/******************************************************************************************/
char init_1wire(void) {
    unsigned char st;


    CLRBIT(OW_TRIS, OW_BIT);
    NOP();    

	CLRBIT(OW_LAT, OW_BIT);
	
    Delay240Us();
    Delay240Us();

    __gie = (unsigned char)GIE;
    GIE = 0;

    SETBIT(OW_TRIS, OW_BIT);

    Delay65Us();

	st = !TESTBIT(OW_PORT, OW_BIT);
	
    GIE = (bit)__gie;

    Delay240Us();

    if(st)
        return(1);
    else
        return(0);
}


/******************************************************************************************/
char read_1wire(void) {
	
    char i, data = 0;

    __gie = (UCHAR)GIE;
    GIE = 0;

    SETBIT(OW_LAT, OW_BIT);

    for(i = 0; i < 8; i++) {
      
        CLRBIT(OW_TRIS, OW_BIT);
		CLRBIT(OW_LAT, OW_BIT);
		
        Delay5Us();

		SETBIT(OW_TRIS, OW_BIT);
		
        Delay14Us();

        data >>= 1;        

        if(TESTBIT(OW_PORT, OW_BIT))data |= 0x80;
        else data &= 0x7f;

        Delay65Us();

    }

    GIE = (bit)__gie;

    return(data);
}

/******************************************************************************************/
void write_1wire(char data) {
    char i;

    __gie = (UCHAR)GIE;
    GIE = 0;

    SETBIT(OW_LAT, OW_BIT);
    
    for(i = 0; i < 8; i++) {

        CLRBIT(OW_TRIS, OW_BIT);
		CLRBIT(OW_LAT, OW_BIT);		
		
        Delay5Us();

		if(TESTBIT(data, i)) SETBIT(OW_LAT, OW_BIT);  else CLRBIT(OW_LAT, OW_BIT);
		
        Delay65Us();

        SETBIT(OW_LAT, OW_BIT);

        Delay50Us();
    }

    GIE = (bit)__gie;

    SETBIT(OW_TRIS, OW_BIT);

}

/******************************************************************************************/
double ds18b20_read(void) {
	
	int t;	
    char i;
    double f;

    SETBIT(OW_TRIS, OW_BIT);
    
    if(init_1wire()) {
        write_1wire(0xcc);
        write_1wire(0xbe);
    } else return OW_ERROR;

#ifndef OW_NO_USE_CHKSM
	char 	dowcrc = 0;
    for(i = 0; i < 9; i++) {
        scratch[i] = read_1wire();
        // crc
        dowcrc = dscrc_table[dowcrc ^ scratch[i]];
    }    
	// return error
    if((dowcrc != 0) || (scratch[7]==0)) return OW_ERROR; 
    
#else
    for(i = 0; i < 2; i++) {
        scratch[i] = read_1wire();
        if((scratch[0]==0xff) && (scratch[1]==0xff)) return OW_ERROR;         
    }
#endif

    //calculate temperature
    t = (scratch[1] << 8) | scratch[0];

    f = (double)(t & 0x000f) * 0.0625;

    f = f + (double)(t >>= 4);

    return f;
}


/******************************************************************************************/
void ds18b20_start(void) {

    if(init_1wire()) {
        write_1wire(0xcc);
        write_1wire(0x44);
    }

    Delay14Us();

    // паразитное питание на время преобразования
    CLRBIT(OW_TRIS, OW_BIT);
    SETBIT(OW_LAT, OW_BIT);

    ///750Ms
    // Пускай питается пока, не спросят

}


#ifdef	OW_USE_CHANGING_BIT


/******************************************************************************************/
void write_1wire_port(char data) {
    char i;

    __gie = (UCHAR)GIE;
    GIE = 0;

    OW_LAT = 0xff;
    
    for(i = 0; i < 8; i++) {

        OW_TRIS = 0x00;
		OW_LAT= 0x00;	
		
        Delay5Us();

		if(TESTBIT(data, i)) OW_LAT = 0xff;  else OW_LAT = 0x00;
		
        Delay65Us();

        OW_LAT = 0xff;

        Delay50Us();
    }

    GIE = (bit)__gie;

    OW_TRIS = 0xff;

}

/******************************************************************************************/
void ds18b20_start_port(void) {	

	//init
	OW_TRIS = 0x00;
    OW_LAT = 0x00;
    Delay240Us();
    Delay240Us();
    OW_TRIS = 0xff;

    Delay65Us();
    Delay240Us();

    write_1wire_port(0xcc);
    write_1wire_port(0x44);


    Delay14Us();

    // паразитное питание на время преобразования
    OW_TRIS = 0x00;
    OW_LAT = 0xff;

    ///750Ms
    // Пускай питается пока, не спросят

}

/******************************************************************************************/
double ds18b20_read_bit(unsigned char b) {
	
	b &= 7;
	ow_bit = b;
	
	return ds18b20_read();
	
}

#endif

/******************************************************************************************/
#ifdef OW_USE_CHARGING_PORT

void ow_set_port(volatile unsigned char *tris, volatile unsigned char *lat, volatile unsigned char *port){
    PTRIS = tris;
    PPORT = port;
	PLAT = lat;		
}

#endif
