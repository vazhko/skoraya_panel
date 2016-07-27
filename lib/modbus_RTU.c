#include	"modbus_RTU.h"

/******************************************************************************************/
extern void putbyte(char byte);


/******************************************************************************************/
void crc_send(unsigned int crc) {
#ifdef  USE_OLD_MODBUS
	putbyte((unsigned char) (crc >> 8));
	putbyte((unsigned char) crc);     
#else
	putbyte((unsigned char) crc);
    putbyte((unsigned char) (crc >> 8));
#endif	
   
}

/******************************************************************************************/
void crc_mb_init(unsigned int *code) {
	*code = 0xffff;
}

/******************************************************************************************/
void crc_mb_init_i(unsigned int *code) {
	*code = 0xffff;
}

/******************************************************************************************/
unsigned char crc_mb_calc_i(unsigned int *code, unsigned char next) {
	unsigned char flag;
	unsigned char i;
	*code ^= next;

	for (i = 0; i < 8; i++) {
		flag = *code & 1;
		*code = *code >> 1;
		if (flag) *code ^= 0xA001;
	}
	return next;
}

/******************************************************************************************/
unsigned char crc_mb_calc(unsigned int *code, unsigned char next) {
	unsigned char flag;
	unsigned char i;
	*code ^= next;

	for (i = 0; i < 8; i++) {
		flag = *code & 1;
		*code = *code >> 1;
		if (flag) *code ^= 0xA001;
	}
	return next;
}

/******************************************************************************************/
unsigned int crc_mb(unsigned char *buffer, unsigned char count) {
	unsigned int CRC;
	unsigned char i;

	crc_mb_init(&CRC);
	for (i = 0; i < count; i ++) {
		crc_mb_calc(&CRC, *buffer++);
	}
	*buffer++ = (unsigned char )(CRC >> 8);
	*buffer     = (unsigned char ) CRC;
	return 	CRC;
}

/******************************************************************************************/
// Проверка CRC  в count - количество байт в buffer не включая CRC

unsigned char crc_mb_check(unsigned char *buffer, unsigned char cnt) {

	unsigned int crc;
	unsigned char i;

	crc_mb_init(&crc);

	for (i = 0; i < cnt; i++) {
		crc_mb_calc(&crc, *buffer++);
	}


#ifdef	USE_NO_CHKSM
	return 1;
#else

#ifdef  USE_OLD_MODBUS
	if (((unsigned char) (crc >> 8) == *buffer++) && ((unsigned char) crc == *buffer)) return 1; // так мы работали в логгере
#else
	if (((unsigned char) (crc) == *buffer++) && ((unsigned char) (crc >> 8) == *buffer)) return 1; // правильно oven
#endif

	return 0;
#endif



}

