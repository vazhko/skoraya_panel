#include <pic18.h>
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "macro.h"



/******************************************************************************************/
#ifndef MAX_UNIPRINT
#define MAX_UNIPRINT 999.9
#endif

#ifndef MIN_UNIPRINT
#define MIN_UNIPRINT (-999.9)
#endif

/******************************************************************************************/
double polynom(WORD adress, const double x, BYTE fit);
double eval_poly_vv(double x, const double *k, BYTE fit);

void replace(char* str, char old, char new);

void print_a_b(char* buf, double f_data, unsigned char a, unsigned char b);
BYTE print_a_b_min_max(char* , double , double , double , unsigned char , unsigned char );

void pwm1_load(WORD load);
void pwm2_load(WORD load);

void T9000_set_channel(BYTE ch);




