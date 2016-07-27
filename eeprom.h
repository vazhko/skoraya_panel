//Generating by Configurator.exe
//Autor Wajko Wladislaw

#ifndef 	EEPROM_H
#define 	EEPROM_H

#define   PW_UNIT_NO    0x0000  // 1
#define   PW_PROBENUMBER1    0x0002  // 1
#define   PW_PROBENUMBER2    0x0004  // 2
#define   PB_DATECAL1    0x0006  // 22
#define   PB_DATECAL2    0x000C  // 22
#define   PF_A1    0x0012  // 0
#define   PF_B1    0x0016  // 1
#define   PF_A2    0x001A  // 0
#define   PF_B2    0x001E  // 1
#define   PF_AC    0x0022  // 0
#define   PF_BC    0x0026  // 1
#define   PB_DATECOOL    0x002A  // 22
#define   PF_A    0x0030  // 0
#define   PF_B    0x0034  // 1
#define   PW_T1MIN    0x0038  // 1800
#define   PW_T1MAX    0x003A  // 3000
#define   PW_TIME1START    0x003C  // 0
#define   PW_T2MIN    0x003E  // 1600
#define   PW_T2MAX    0x0040  // 2800
#define   PW_TIME2START    0x0042  // 0
#define   PF_P1    0x0044  // 0.65
#define   PF_I1    0x0048  // 0.054
#define   PF_D1    0x004C  // 1.9
#define   PF_P2    0x0050  // 0.65
#define   PF_I2    0x0054  // 0.014
#define   PF_D2    0x0058  // 4
#define   PF_R_REF    0x005C  // 6000
#define   PF_TASK_TC    0x0060  // 8
#define   PF_TASK_TH    0x0064  // 32
#define   PW_TSTART    0x0068  // 2800
#define   PB_LANG    0x006A  // 2
#define   PF_MAX1    0x006B  // 0.99
#define   PF_MAX2    0x006F  // 0.99
#define   PB_SCALE_MODE    0x0073  // 0
#define   PB_SENSOR_NO    0x0074  // 0

/******************************************************************************************/
#define 	EEPROM_INIT \
__EEPROM_DATA(0x01,0x00,0x01,0x00,0x02,0x00,0x16,0x06);\
__EEPROM_DATA(0x25,0x0B,0x16,0x0F,0x16,0x06,0x25,0x0A);\
__EEPROM_DATA(0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x80,0x3F,0x16,0x06,0x25,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F);\
__EEPROM_DATA(0x08,0x07,0xB8,0x0B,0x00,0x00,0x40,0x06);\
__EEPROM_DATA(0xF0,0x0A,0x00,0x00,0x66,0x66,0x26,0x3F);\
__EEPROM_DATA(0x1B,0x2F,0x5D,0x3D,0x33,0x33,0xF3,0x3F);\
__EEPROM_DATA(0x66,0x66,0x26,0x3F,0x42,0x60,0x65,0x3C);\
__EEPROM_DATA(0x00,0x00,0x80,0x40,0x00,0x80,0xBB,0x45);\
__EEPROM_DATA(0x00,0x00,0x00,0x41,0x00,0x00,0x00,0x42);\
__EEPROM_DATA(0xF0,0x0A,0x02,0xA4,0x70,0x7D,0x3F,0xA4);\
__EEPROM_DATA(0x70,0x7D,0x3F,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);\

// Default EEPROM values
#define    EE_DATA  \
 {0x01,0x00,0x01,0x00,0x02,0x00,0x16,0x06,\
  0x25,0x0B,0x16,0x0F,0x16,0x06,0x25,0x0A,\
  0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x80,0x3F,0x16,0x06,0x25,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,\
  0x08,0x07,0xB8,0x0B,0x00,0x00,0x40,0x06,\
  0xF0,0x0A,0x00,0x00,0x66,0x66,0x26,0x3F,\
  0x1B,0x2F,0x5D,0x3D,0x33,0x33,0xF3,0x3F,\
  0x66,0x66,0x26,0x3F,0x42,0x60,0x65,0x3C,\
  0x00,0x00,0x80,0x40,0x00,0x80,0xBB,0x45,\
  0x00,0x00,0x00,0x41,0x00,0x00,0x00,0x42,\
  0xF0,0x0A,0x02,0xA4,0x70,0x7D,0x3F,0xA4,\
  0x70,0x7D,0x3F,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

// Write enable flags
#define IS_WRITEBLE  \
 0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL,0x0000UL


/******************************************************************************************/
void write___ee(unsigned int adr, unsigned char *var, unsigned char byte);
void read___ee(unsigned int adr, unsigned char *var, unsigned char byte);
void set_ee_default(void);	

/******************************************************************************************/
#define RAM_TO_EE(adr, var) write___ee(adr, (char *)&var, sizeof(var))
#define EE_TO_RAM(adr, var)  read___ee(adr, (char *)&var, sizeof(var))

#define BUFF_TO_EE(adr, buff, n) write___ee(adr, (char *)&buff, n)
#define EE_TO_BUFF(adr, buff, n)  read___ee(adr, (char *)&buff, n)

/******************************************************************************************/	
unsigned char is_ee_wr(unsigned int adr);

/******************************************************************************************/

#endif
