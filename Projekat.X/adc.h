#ifndef ADC_H
#define ADC_H
#include<p30fxxxx.h>

// touchscreen driver koristi ove kako bi odredio poziciju dodira
extern unsigned int sirovi0,sirovi1;

// Podesi pinove za rad sa ADCom
void ConfigureADCPins(void);

// Podesi ADC
void ADCinit(void);

#endif
