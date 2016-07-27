#include	<math.h>
#include	"sub18.h"


/******************************************************************************************/
void read_float_ee(BYTE pEE, double *pFl) {

    BYTE j, *pi;
    pi = (BYTE*)pFl;

    for(j = 0; j <= 3; j++) {
        *pi++ = EEPROM_READ(pEE++);

    }
}
/******************************************************************************************/
//
double pow_i(double x, BYTE y) {
    double f = 1.0;

    if(y == 0) return f;

    do {
        f *= x;
        y --;
    } while(y);

    return f;
}

/******************************************************************************************/
// ‘унк полином начинает считывание EEPROM с последнего коэфф.
double polynom(WORD adress, double x, BYTE fit) {
    double f_temp_1, f_temp_2 = 0;
    BYTE i = 0;

    do {
        read_float_ee(adress, &f_temp_1);
        f_temp_2 = f_temp_2 + pow_i(x, i) * f_temp_1;
        adress += 4;
    } while(i ++ < fit);

    return  f_temp_2;

}


/******************************************************************************************/
void replace(char* str, char old, char new) {
    do {
        if(*str == old)*str = new;
    } while(*++str);
}



/*************************************************************************************/
// ”ниверсальна€ печать
/*************************************************************************************/
BYTE print_a_b_min_max(char* buf, double f_data, double f_data_min, double f_data_max, unsigned char a, unsigned char b) {
    BYTE i;

    if((f_data >= f_data_max) || (f_data <= f_data_min)) {
        i = a + b + 1;
        buf[i + 1] = 0;
        buf[i] = ' ';  
        for(i; i; i--) buf[i] = '-';      
        buf[a + 1] = '.';
        buf[0] = ' ';
        return 0;
    }
    print_a_b(buf, f_data, a, b);
    return 1;
}

/*************************************************************************************/
void print_a_b(char* buf, double f_data, unsigned char a, unsigned char b) {
    BYTE i, sign;

    if(f_data < 0) {
        sign = 1;
        f_data = (-1.0) * f_data;
    } else {
        sign = 0;
    }
    
    if(b == 0) f_data +=0.5;
    else if(b == 1) f_data +=0.05;
    else if(b == 2) f_data +=0.005;
    
    //f_data += (0.5 / pow_i(10, b));

    i = b + 1;
    for(i; i; i--) f_data *= 10.0;

    i = a + b + 3;
    while(i) {
        i --;
        if(i == (a + 1)) continue;
        buf[i] = (char)((long int)f_data % 10) + '0';
        f_data /= 10;
    }

    if(buf[0] - '0')goto skip; // если большое положительное

    // удаление лишних нулей и добавление знака
    for(i = 0; i <= a; i++) {
        if(((buf[i] - '0') == 0) && (i != a)) buf[i] = ' ';
        else {
            if(sign) buf[i - 1] = '-';
            goto skip;
        }
    }

skip:

    buf[a + 1] = '.';
    buf[a + b + 2] = 0;

}

/***************************************************************************************/
void pwm1_load(WORD load) {
    CCPR1L = ((BYTE)((load) >> 2));
    DC1B1 = (bit)((load) >> 1);
    DC1B0 = (bit)(load);
}

/***************************************************************************************/
void pwm2_load(WORD load) {
    CCPR2L = ((BYTE)((load) >> 2));
    DC2B1 = (bit)((load) >> 1);
    DC2B0 = (bit)(load);
}


/***************************************************************************************/
void T9000_set_channel(BYTE ch) {

    TRISB &= 0b11000111;

    if TESTBIT(ch, 0) SETBIT(LATB, 3);
    else CLRBIT(LATB, 3);
    if TESTBIT(ch, 1) SETBIT(LATB, 4);
    else CLRBIT(LATB, 4);
    if TESTBIT(ch, 2) SETBIT(LATB, 5);
    else CLRBIT(LATB, 5);
}
