/*
$Id: misc.c 202 2014-12-01 Vlad $
 */

#include "Main.h"
#include "math.h"


/******************************************************************************************/
void pwm4_load(WORD load) {

    if(load & 1) CCP4CONbits.DC4B0 = 1;
    else CCP4CONbits.DC4B0 = 0;
    if(load & 2) CCP4CONbits.DC4B1 = 1;
    else CCP4CONbits.DC4B1 = 0;

    CCPR4L = load >> 2;
}



//double temp_t;
/****************************************************************************/
void InitializeSystem(void) {

#ifdef MTOUCH_DEBUG
    // This structure will contain the optimal CTMU current.
    volatile DEBUGCURRENT* pOptimalCurrent;
    // This structure will contain the optimal CTMU charge delay.
    volatile DEBUGDELAY* pDelay;
    // This variable will contain the optimal threshold.
    volatile UINT16     optimalThreshold;

#endif

    unsigned int pll_startup_counter = 600;

    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
    while(pll_startup_counter--);



    RCONbits.IPEN = 1;

    // Подобрано 1ms
    T2CONbits.T2CKPS = 3;
    PR2 = 200;
    T2CONbits.T2OUTPS = 3;
    T2CONbits.TMR2ON = 1;
    IPR1bits.TMR2IP = 1;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;


    T4CONbits.T4CKPS = 3;
    PR4 = 100;
    T4CONbits.T4OUTPS = 0;
    T4CONbits.TMR4ON = 1;
    IPR5bits.TMR4IP = 1;
    PIR5bits.TMR4IF = 0;
    PIE5bits.TMR4IE = 1;

    ///INTCONbits.PEIE = 1;
    ///INTCONbits.GIE = 1;
    ///while(1) NOP();

    // STEP 1
    // mTouch library initialization.
    MTouchInit();

    ANSELA = 0b00000000;
    ANSELB = 0b00000000;
    ANSELC = 0b00000000;
    ANSELD = 0b00000000;
    ANSELE = 0b00000000;

    // STEP 2
    // Sensors initialization. All sensors must be initialized (see MTOUCH_SENSORS_NUMBER in mTouchConfig.h).
    MTouchSetSensor(0, CH0_TRIS, CH0_LAT, CH0_IO_BIT, CH0_AN_NUM, -1, -1, -1);
    MTouchSetSensor(1, CH1_TRIS, CH1_LAT, CH1_IO_BIT, CH1_AN_NUM, -1, -1, -1);
    MTouchSetSensor(2, CH2_TRIS, CH2_LAT, CH2_IO_BIT, CH2_AN_NUM, -1, -1, -1);
    MTouchSetSensor(3, CH3_TRIS, CH3_LAT, CH3_IO_BIT, CH3_AN_NUM, -1, -1, -1);
    MTouchSetSensor(4, CH4_TRIS, CH4_LAT, CH4_IO_BIT, CH4_AN_NUM, -1, -1, -1);
    MTouchSetSensor(5, CH5_TRIS, CH5_LAT, CH5_IO_BIT, CH5_AN_NUM, -1, -1, -1);
    MTouchSetSensor(6, CH6_TRIS, CH6_LAT, CH6_IO_BIT, CH6_AN_NUM, -1, -1, -1);
    MTouchSetSensor(7, CH7_TRIS, CH7_LAT, CH7_IO_BIT, CH7_AN_NUM, -1, -1, -1);
    MTouchSetSensor(8, CH8_TRIS, CH8_LAT, CH8_IO_BIT, CH8_AN_NUM, -1, -1, -1);
    MTouchSetSensor(9, CH9_TRIS, CH9_LAT, CH9_IO_BIT, CH9_AN_NUM, -1, -1, -1);
    MTouchSetSensor(10, CH10_TRIS, CH10_LAT, CH10_IO_BIT, CH10_AN_NUM, -1, -1, -1);

#ifdef MTOUCH_DEBUG
    /*
        // Debug log initialization (allows streaming data to PC GUI). PC GUI helps to
        // choose right values for sensors' oversampling factors and thresholds.
        SysLogInit();

    	// MTouchDebugDelay(sensorNumber) function calculates an optimal CTMU charge delay value
        // to provide charging sensor about 75% of AVdd.
        pDelay = MTouchDebugDelay(0);
        // Set the adjusted optimal CTMU charge delay value for the sensor.
        MTouchSetChargeDelay(0, pDelay->delay);

        pDelay = MTouchDebugDelay(1);
        MTouchSetChargeDelay(1, pDelay->delay);
        pDelay = MTouchDebugDelay(2);
        MTouchSetChargeDelay(2, pDelay->delay);
        pDelay = MTouchDebugDelay(3);
        MTouchSetChargeDelay(3, pDelay->delay);
        pDelay = MTouchDebugDelay(4);
        MTouchSetChargeDelay(4, pDelay->delay);
        pDelay = MTouchDebugDelay(5);
        MTouchSetChargeDelay(5, pDelay->delay);
    */

#endif

    // MTouchSetButton(0,              // button number
    //                 5,              // sensor number
    //                 DECODE_TOGGLE); // decode method (see MTOUCHCONTROLDECODE enumeration in mTouchControl.h)

    MTouchSetButton(KEY_0_AVT_PWR_220, 0, DECODE_ONE_EVENT);    
    MTouchSetButton(KEY_1_LT_NOSHY, 1, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_2_LT_SALON_M, 2, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_3_LT_SALON_P, 3, DECODE_ONE_EVENT);    
    MTouchSetButton(KEY_4_VENT_OUT, 4, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_5_VENT_IN, 5, DECODE_ONE_EVENT);    
    MTouchSetButton(KEY_6_ROZ_ON, 6, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_7_LT_OUT_BACK, 7, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_8_LT_OUT_SIDE, 8, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_9_LT_SHELF_M, 9, DECODE_ONE_EVENT);
    MTouchSetButton(KEY_10_LT_SHELF_P, 10, DECODE_ONE_EVENT);
    

    MTouchSetButton(KEY_2_LT_SALON_MM, 2, DECODE_PRESS_REPEAT | DECODE_ONE_EVENT);
    MTouchSetButton(KEY_3_LT_SALON_PP, 3, DECODE_PRESS_REPEAT | DECODE_ONE_EVENT);   

    uart16_init();
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    RCIP = 0;
    RCIE = 1;
    
    LED1_AVT_PWR_TRIS = 0;
    LED2_ROZ_TRIS = 0;
    LED4_LT_BACK_TRIS = 0;
    LED5_LT_SIDE_TRIS = 0;
    
    LED1_AVT_PWR = 0;
    LED2_ROZ = 0;
    LED4_LT_BACK = 0;
    LED5_LT_SIDE = 0;     
    
      
    BK_ON_TRIS = 0;
    BK_ON = 1;


    INTCONbits.PEIE = 1;
    
    // после инициал всего
    //	INTCONbits.GIE = 1;


}//end InitializeSystem





/******************************************************************************************/
#ifdef MTOUCH_DEBUG

void Sensor_Optimization() {

}

void LogChar(char ch) {
    while(TXSTA1bits.TRMT == 0) Nop();
    TXREG1 = ch;
}

#endif


/******************************************************************************************/
#define SysDelay1Ms()  { UINT16 d=1067; while(d--){Nop();} }

void SysDelayMs(UINT16 delay) {
    while(delay--) {
        SysDelay1Ms();
    }
}


