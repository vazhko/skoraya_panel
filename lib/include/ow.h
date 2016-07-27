#ifndef OW_H
#define OW_H

// 21.06.2015
// Add realtime changing port and pin
// ds18b20_start_port
// ow_set_port
// ds18b20_read_bit

#if defined(_18F4520) || defined(_18F452) || defined(_18F46K22)

#if GetSystemClock() == 64000000ul
#define	Delay100Us()	ow_delay(120)
#define	Delay50Us()		ow_delay(60)
#define	Delay5Us()		ow_delay(5)

#define	Delay65Us()		ow_delay(78)
#define	Delay240Us()	ow_delay(295)
#define	Delay14Us()		ow_delay(16)

#elif GetSystemClock() == 40000000ul
#define	Delay100Us()	ow_delay(76)
#define	Delay50Us()		ow_delay(37)
#define	Delay5Us()		ow_delay(2)

#define	Delay65Us()		ow_delay(49)
#define	Delay240Us()	ow_delay(186)
#define	Delay14Us()		ow_delay(10)


#elif GetSystemClock() == 32000000ul

#define	Delay100Us()	ow_delay(60)
#define	Delay50Us()		ow_delay(30)
#define	Delay5Us()		ow_delay(2)

#define	Delay65Us()		ow_delay(29)
#define	Delay240Us()	ow_delay(195)
#define	Delay14Us()		ow_delay(7)

#endif

#elif defined (_18F6527)
    #if GetSystemClock() == 32000000ul
	    
	    #define	Delay100Us()	ow_delay(72)
	    #define	Delay50Us()		ow_delay(36)
	    #define	Delay5Us()		ow_delay(3)
	
	    #define	Delay65Us()		ow_delay(47)
	    #define	Delay240Us()	ow_delay(172)
	    #define	Delay14Us()		ow_delay(9)
	    
	 // раб   
     #elif GetSystemClock() == 16000000ul
	    
	    #define	Delay100Us()	ow_delay(36)
	    #define	Delay50Us()		ow_delay(16)
	    #define	Delay5Us()		Nop(),Nop()

	
	    #define	Delay65Us()		ow_delay(23)
	    #define	Delay240Us()	ow_delay(86)
	    #define	Delay14Us()		ow_delay(4)	       

	 // отладить   
     #elif GetSystemClock() == 8000000ul
	    
	    #define	Delay100Us()	ow_delay(18)
	    #define	Delay50Us()		ow_delay(8)
	    #define	Delay5Us()		Nop(),Nop()
	    
	    #define	Delay65Us()		ow_delay(11)
	    #define	Delay240Us()	ow_delay(43)
	    #define	Delay14Us()		ow_delay(1)	
    #endif
#endif


#ifndef	OW_BIT
#define	OW_USE_CHANGING_BIT
#define	OW_BIT		ow_bit
#endif


#ifndef OW_TRIS
#define	OW_USE_CHARGING_PORT
#define	OW_TRIS		(*PTRIS)
#define	OW_LAT		(*PLAT)
#define	OW_PORT		(*PPORT)
#endif

#define	OW_ERROR		-60.0


void ow_delay(unsigned int del);


double ds18b20_read(void);
void ds18b20_start(void);

void ds18b20_start_port(void);
double ds18b20_read_bit(unsigned char b);

void ow_set_port(volatile unsigned char *tris, volatile unsigned char *lat, volatile unsigned char *port);


#endif
