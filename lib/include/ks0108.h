#include 	<pic18.h>
#include 	<stdlib.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "delay.h"



// put to HardwareProfile.h
//  KS0108
//#define LCD_TRIS		TRISE
//#define LCD_DATA		PORTE
//#define LCD_LAT			LATE
//
//#define LCD_ENABLE        LATG0
//#define LCD_RW            LATG1
//#define LCD_DI            LATG2
//	
//#define LCD_CS1           LATD1	
//#define LCD_CS2           LATD0
//#define LCD_RESET         LATD2
//
//#define LCD_DATA_IN		PORTE		// Data Input Register
//#define LCD_DATA_OUT		LATE		// Data Output Register
//#define LCD_DATA_DIR		TRISE		// Data Direction Register for Data Port



// Function Paramters
#define INCREMENT_X			0
#define NO_INCREMENT_X		1

// Command Port Bits
#define D_I					0x00		// D/I Bit Number
#define R_W					0x01		// R/W Bit Number
#define EN					0x02		// EN Bit Number
#define CSEL1				0x03		// CS1 Bit Number
#define CSEL2				0x04		// CS2 Bit Number

// Chips
#define CHIP1				0x00
#define CHIP2				0x01

// Commands
#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_SET_ADD			0x40
#define LCD_SET_PAGE		0xB8
#define LCD_DISP_START		0xC0

// Fill Modes
#define BLACK				0xFF
#define CLEAR				0x00


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t page;
} displayPos;


// надпись

typedef struct {
    BYTE x;
    BYTE y;
    BYTE size;
    const char *p_font;
    char str[31];
    //char *str;
} Label;



// Function Prototypes
void ks0108Enable(void);
void ks0108Fill(uint8_t mode);
void ks0108GotoXY(uint8_t, uint8_t);
void ks0108Init(void);

BYTE ks0108PutChar(char s, const char *p_font, char mode);
void ks0108PutString(char *string, const char *p_font, char mode);
char ks0108ReadData(uint8_t incXAdd);
void ks0108WriteCommand(uint8_t cmd, uint8_t chip);
void ks0108WriteData(uint8_t data);


void ks0108Fill_xy(uint8_t mode, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void ks0108Line(BYTE xn, BYTE yn, BYTE xk, BYTE yk);
void ks0108Rect(BYTE x, BYTE y, BYTE dx, BYTE dy);

void LabelCreate(Label *lb, BYTE x, BYTE y, const char *font, BYTE ln);
void LabelSetTxt(Label *lb, char *s);
void LabelClrTxt(Label *lb);

#define USE_LCD_MEM
// инвертировать
//#define USE_INVERSE

