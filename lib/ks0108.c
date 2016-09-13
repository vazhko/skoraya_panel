#include "ks0108.h"

/******************************************************************************************/

void ks0108SetDot(uint8_t, uint8_t);
void ks0108ClearDot(uint8_t, uint8_t);

/******************************************************************************************/
displayPos pos;

#ifndef USE_LCD_MEM

volatile BYTE lcd_ram[1024];
volatile WORD ram_pos;

#endif

/******************************************************************************************/
void ks0108Enable(void) {
    DelayUs(5);
    LCD_ENABLE = 1;
    DelayUs(3);
    //Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    LCD_ENABLE = 0;
    //Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    DelayUs(3);
    //DelayUs(10);
}

/******************************************************************************************/
void ks0108Fill(uint8_t mode) {
    uint8_t i, j;

    for(i = 0; i < 8; i++) { // page address
        ks0108GotoXY(0, i * 8);
        for(j = 0; j < 128; j++) // x address
            ks0108WriteData(mode); // write data			
    }
}

/******************************************************************************************/
void ks0108Fill_xy(uint8_t mode, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t i, j;

    for(i = y1; i < y2; i++) { // page address
        ks0108GotoXY(x1, i * 8);
        for(j = x1; j < x2; j++) // x address
            ks0108WriteData(mode); // write data			
    }
}

/******************************************************************************************/
void ks0108GotoXY(uint8_t x, uint8_t y) {
    uint8_t chip = CHIP1, cmd;

    if(x > 127) x = 0; // ensure that coordinates are legal
    if(y > 63) y = 0;

    pos.x = x; // save new coordinates
    pos.y = y;
    pos.page = y >> 3;

    if(x >= 64) { // select the right chip
        x -= 64;
        chip = CHIP2;
    }

#ifndef USE_LCD_MEM	
    ram_pos = 512 * chip + x * 8 + pos.page;
#endif


    cmd = LCD_SET_ADD | x;
    ks0108WriteCommand(cmd, chip); // set x address on active chip

    cmd = LCD_SET_PAGE | pos.page; // set y address on both chips

    ks0108WriteCommand(cmd, CHIP1);
    ks0108WriteCommand(cmd, CHIP2);



}

/******************************************************************************************/
void ks0108SetDot(uint8_t x, uint8_t y) {
    uint8_t data;

    ks0108GotoXY(x, y); // read data from display memory
    data = ks0108ReadData(NO_INCREMENT_X); // dummy read
    data = ks0108ReadData(NO_INCREMENT_X); // "real" read	

    if(!(data & (0x01 << (y % 8)))) {

        data |= 0x01 << (y % 8); // set dot	

        ks0108WriteData(data); // write data back to display

    }
}

/******************************************************************************************/
void ks0108ClearDot(uint8_t x, uint8_t y) {
    uint8_t data;

    ks0108GotoXY(x, y); // read data from display memory
    data = ks0108ReadData(NO_INCREMENT_X); // dummy read
    data = ks0108ReadData(NO_INCREMENT_X); // "real" read

    if((data & (0x01 << (y % 8)))) {

        data &= ~(0x01 << (y % 8)); // clear dot

        ks0108WriteData(data); // write data back to display
    }
}

//BYTE ddd = 0;

/******************************************************************************************/
void ks0108Init(void) {
#ifndef USE_LCD_MEM
    WORD i;
#endif    

    LCD_CS1_TRIS = 0;
    LCD_CS2_TRIS = 0;
    LCD_RESET_TRIS = 0;

    LCD_ENABLE_TRIS = 0;
    LCD_RW_TRIS = 0;
    LCD_DI_TRIS = 0;

    LCD_RESET = 0;
    DelayMs(2);
    LCD_RESET = 1;
    DelayMs(1);

    pos.x = 0;
    pos.y = 0;
    pos.page = 0;


    ks0108WriteCommand(LCD_ON, CHIP1); // power on
    ks0108WriteCommand(LCD_ON, CHIP2);

    ks0108WriteCommand(LCD_DISP_START, CHIP1); // display start line = 0
    ks0108WriteCommand(LCD_DISP_START, CHIP2);
    ks0108Fill(CLEAR); // display clear

#ifndef USE_LCD_MEM

    for(i = 0; i < 1024; i++)lcd_ram[i] = 0;
    for(i = 0; i < 512; i++) lcd_ram[i] = ks0108ReadData(0);

#endif	



}

/******************************************************************************************/
char ks0108ReadData(uint8_t incXAdd) {
    uint8_t data;
    volatile uint8_t i;



#ifdef	USE_LCD_MEM	

    LCD_DATA_OUT = 0x00;
    LCD_DATA_DIR = 0xff; // data port is input

    if(pos.x < 64) {
        LCD_CS1 = 1;
        LCD_CS2 = 0;
    } else if(pos.x >= 64) {
        LCD_CS1 = 0;
        LCD_CS2 = 1;
    }

    LCD_RW = 1;
    LCD_DI = 1;

    for(i = 0; i < 8; i++);

    LCD_ENABLE = 1; // EN high level width: min. 450ns

    for(i = 0; i < 8; i++);

    data = LCD_DATA_IN; // read Data	
#ifdef	USE_INVERSE
    data ^= 0xff;
#endif			 

    LCD_ENABLE = 0;

    for(i = 0; i < 8; i++);

#else

    data = lcd_ram[ram_pos];

#endif


    LCD_DATA_DIR = 0x00;


    if(incXAdd == INCREMENT_X) // display x-address is incremented automatically
        pos.x++;
    else
        ks0108GotoXY(pos.x, pos.y);


    return data;
}

/******************************************************************************************/
void ks0108WriteCommand(uint8_t cmd, uint8_t chip) {

    if(chip == CHIP1) {
        LCD_CS1 = 1;
        LCD_CS2 = 0;
    } else if(chip == CHIP2) {
        LCD_CS1 = 0;
        LCD_CS2 = 1;
    }

    LCD_RW = 0;
    LCD_DI = 0;

    LCD_DATA_DIR = 0x00; // data port is output
    LCD_DATA_OUT = cmd; // write command
    ks0108Enable(); // enable
    LCD_DATA_OUT = 0x00;
}

/******************************************************************************************/
void ks0108WriteData(uint8_t data) {

    if(pos.x >= 128)
        return;

    if(pos.x < 64) {
        LCD_CS1 = 1;
        LCD_CS2 = 0;
    } else if(pos.x >= 64) {
        LCD_CS1 = 0;
        LCD_CS2 = 1;
    }

    if(pos.x == 64) { // chip2 X-address = 0
        ks0108WriteCommand(LCD_SET_ADD, CHIP2);
#ifndef USE_LCD_MEM		
        ram_pos += 512;
#endif
    }


    LCD_RW = 0;
    LCD_DI = 1;

    LCD_DATA_DIR = 0x00; // data port is output



#ifdef	USE_INVERSE
    LCD_DATA_OUT = data ^ 0xff;
#else
    LCD_DATA_OUT = data; // write data	
#endif
    ks0108Enable();


#ifndef USE_LCD_MEM	
    lcd_ram[ram_pos] = data;
#endif

    pos.x++;

    LCD_DATA_OUT = 0;
}

/******************************************************************************************/
BYTE ks0108PutChar(char s, const char *p_font, char mode) {

    WORD base;
    BYTE x_, y_;
    BYTE xCnt, yCnt, x = 0, y, chWidth;
    BYTE temp = 0, mask, _fontHeight, _fontWidth;
    const char *pChImage;

    _fontHeight = p_font[0];
    _fontWidth = p_font[1];

    base = s - 32;
    base *= p_font[2]; // количю байт на символ
    base += 3;


    chWidth = p_font[base];
    pChImage = &p_font[base + 1];

    y_ = pos.y;
    x_ = pos.x;

    y = y_;

    for(yCnt = 0; yCnt < _fontHeight; yCnt++) {
        x = x_;
        mask = 0;
        for(xCnt = 0; xCnt < _fontWidth; xCnt++) {
            if(mask == 0) {
                temp = *pChImage++;
                mask = 0x80;
            }

            if(temp & mask) {
                if(mode == 1)
                    ks0108SetDot(x, y);
                else if(mode == 0)
                    ks0108ClearDot(x, y);
            } else {
                ///ks0108ClearDot(x, y);
            }

            x++;
            mask >>= 1;
        }
        y++;
    }

    // move cursor

    pos.x = x_ + chWidth; // сдвигаем х на ширину
    pos.y = y_;

    return chWidth;
}

/******************************************************************************************/
void ks0108PutString(char *string, const char *p_font, char mode) {
    uint8_t i = 0;
    char c = string[0];

    while(c != 0) {
        ks0108PutChar(c, p_font, mode);
        c = string[++i];
    }
}

/******************************************************************************************/
void ks0108Line(BYTE xn, BYTE yn, BYTE xk, BYTE yk) {
    BYTE dx, dy, s;

    /* Упорядочивание координат и вычисление приращений */
    if(xn > xk) {
        s = xn;
        xn = xk;
        xk = s;
        //s= yn;  yn= yk;  yk= s;
    }
    if(yn > yk) {
        s = yn;
        yn = yk;
        yk = s;
        //s= yn;  yn= yk;  yk= s;
    }
    dx = (xk - xn);
    dy = (yk - yn);
    if((dx == 0) && (dy == 0)) return;

    /* Собственно генерация вектора */
    if(dx >= dy) {
        s = 0;
        while(xn < xk) {
            ks0108SetDot(xn++, yn);
            s = s + dy;
            if(s >= dx) {
                s = s - dx;
                yn++;
            }
        }
    } else {
        s = 0;
        while(yn < yk) {
            ks0108SetDot(xn, yn++);
            s = s + dx;
            if(s >= dy) {
                s = s - dy;
                xn++;
            }
        }
    }
}

/******************************************************************************************/
void ks0108Rect(BYTE x, BYTE y, BYTE dx, BYTE dy) {
    BYTE _x, _y;
    for(_y = y; _y < (y + dy); _y++) {
        for(_x = x; _x < (x + dx); _x++) {
            //ks0108ClearDot(_x, _y);
            ks0108SetDot(_x, _y);
        }
    }

}

/******************************************************************************************/
void LabelCreate(Label *lb, BYTE x, BYTE y, const char *font, BYTE ln) {
    BYTE i;

    lb -> size = ln;
    lb -> p_font = font;
    lb -> x = x;
    lb -> y = y;

    for(i = 0; i < lb -> size; i++) lb -> str[i] = ' ';

    LabelSetTxt(lb, lb -> str);

}

/******************************************************************************************/

/*
void LabelSetTxt(Label *lb, char *str){
    BYTE i, w, end = 0;


    Nop();
    Nop();
    ks0108GotoXY(lb -> x, lb -> y);
    for(i = 0; i < lb -> size; i ++){

        if(str[i] == 0){
            end = 1;
        }
        if(end == 1){
            str[i] = ' ';
        }

        Nop();
        Nop();

        if(lb -> str[i] != str[i]){

            Nop();
            Nop();
            // стираем предыдущий символ
            w = ks0108PutChar(lb -> str[i], lb -> p_font, 0);
            // возврящаем курсор
            pos.x -= w;
            // печатаем новый
            lb -> str[i] = str[i];
            ks0108PutChar(lb -> str[i], lb -> p_font, 1);
        } else {
            // просто перемещаем курсор
            ks0108PutChar(str[i], lb -> p_font, 2);
        }
    }
}
 */

void LabelSetTxt(Label *lb, char *str) {
    BYTE i, SymbWidth, temp = 0, same_symb_count;

    same_symb_count = 0;
    /*
    for(i = 0; i < lb -> size; i++){
        if(lb -> str[i] == str[i]){
            same_symb_count ++;
        } else {
            break;
        }
    }
    */
    
    ks0108GotoXY(lb -> x, lb -> y);
    for(i = same_symb_count; i < lb -> size; i++) {
        SymbWidth = ks0108PutChar(lb -> str[i], lb -> p_font, 0);
    }

    ks0108GotoXY(lb -> x, lb -> y);
    for(i = same_symb_count; i < lb -> size; i++) {
        if(str[i] == 0) temp = 1;
        if(temp == 0) {
            lb -> str[i] = str[i];
        } else {
            // стираем предыдущий символ
            // возврящаем курсор
            // печатаем новый
            lb -> str[i] = ' ';
        }
        ks0108PutChar(lb -> str[i], lb -> p_font, 1);
        // просто перемещаем курсор
    }

}

/******************************************************************************************/
void LabelClrTxt(Label *lb){
		
	BYTE i, SymbWidth;		
	
	ks0108GotoXY(lb -> x, lb -> y);	
	for(i = 0; i < lb -> size; i ++) {
		SymbWidth = ks0108PutChar(lb -> str[i], lb -> p_font, 0);
	}	
	
}


