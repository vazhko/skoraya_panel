#include 	"HardwareProfile.h"

extern void crc_mb_init(unsigned int *);
extern unsigned char crc_mb_calc(unsigned int *, unsigned char);
extern unsigned char crc_mb_check(unsigned char *, unsigned char);

extern void crc_mb_init_i(unsigned int *);
extern unsigned char crc_mb_calc_i(unsigned int *, unsigned char);
unsigned int crc_mb(unsigned char *buffer, unsigned char count);

void crc_send(unsigned int crc);


#ifdef  USE_OLD_MODBUS
	//if (((unsigned char) (crc >> 8) == *buffer++) && ((unsigned char) crc == *buffer)) return 1; // так мы работали в логгере
	#define CRC_1BYTE 	((unsigned char)(crc >> 8))
	#define CRC_2BYTE  	((unsigned char) crc)		
#else
	//if (((unsigned char) (crc) == *buffer++) && ((unsigned char) (crc >> 8) == *buffer)) return 1; // правильно oven
	#define CRC_2BYTE 	((unsigned char)(crc >> 8))
	#define CRC_1BYTE  	((unsigned char) crc)
#endif

