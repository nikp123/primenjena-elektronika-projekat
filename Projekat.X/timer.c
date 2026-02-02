#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <p30fxxxx.h>

#include "alarm.h"

#define TMR2_period 10000 /*  Fosc = 10MHz,
					       1/Fosc = 1us !!!, 1us * 1000 = 1ms  */

uint16_t brojac_ms;
uint8_t brojac_stepper;
unsigned int sekund;

bool blown = false;

uint8_t millis_timer1;
bool stepper_engaged;

void timer_init_2(void)
{
	TMR2 = 0;
	PR2 = TMR2_period;
	
	T2CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	//IPC1bits.T2IP = 3 // T2 interrupt pririty (0-7)
	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
	IFS0bits.T2IF = 0; // clear interrupt flag
	IEC0bits.T2IE = 1; // enable interrupt

	T2CONbits.TON = 1; // T2 on 
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) // svakih 1ms
{
    TMR2 = 0; // Resetuj brojac

    brojac_ms++; // Brojac milisekundi
    brojac_stepper++;

    if (brojac_ms==1000) { // Brojac za sekunde
        brojac_ms=0;
        sekund++;//fleg za sekundu
    }
    
    if(brojac_stepper==20) {
        brojac_stepper = 0;
    }
    
    LATBbits.LATB11 = (stepper_engaged ? (brojac_stepper < 2) : (brojac_stepper < 1));
  
    // Ukoliko je zvucni alarm ukljucen
    // Osciliraj pin A11 (buzzer) brzinom ovog prekida
    // To odgovara zvucnoj frekvenciji: 1kHZ / 2 = 0.5 KHz
    if(alarm_ukljucen()) {
      LATAbits.LATA11 = brojac_ms;
    }  
  
    if(sekund == 60 && alarm_is_armed()) {
        blown = true;
    }

    IFS0bits.T2IF = 0; 
}

void timer_reset() {
    sekund = 0;    // resetuj brojac sa sekunde
    blown = false; // nije istekao brojac vise
}

bool timer_blew() {
    return blown;
}

unsigned int timer_ms() {
    return brojac_ms;
}

void Delay(unsigned int N) {
	unsigned int i;
	for(i=0;i<N;i++);
}

//---------------------------------------


#endif
