/* 
 * File:   touchscreen.c
 * Author: student
 *
 * Created on 04. decembar 2025., 12.37
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined (__dsPIC30F__)
#include <p30fxxxx.h>
#endif

#include "adc.h"
#include "timer.h"
#include "touchscreen.h"

//#define DRIVE_A PORTBbits.RB10
//#define DRIVE_B PORTCbits.RC13
#define DRIVE_A PORTCbits.RC13
#define DRIVE_B PORTCbits.RC14

void touchscreen_configure_pins(void)
{
	//ADPCFGbits.PCFG10=1;
	//ADPCFGbits.PCFG7=digital;

	//TRISBbits.TRISB10=0;
	TRISCbits.TRISC13=0;
    TRISCbits.TRISC14=0;
	
	//LATCbits.LATC14=0;
	//LATCbits.LATC13=0;
}

void touchscreen_coords(unsigned int *X, unsigned int *Y) {
    unsigned int x_vrednost, y_vrednost;
    
    // vode horizontalni tranzistori
	DRIVE_A = 1;  
	DRIVE_B = 0;
    LATCbits.LATC13=1;
    LATCbits.LATC14=0;

    // Cekamo ADC da odradi prevod vrednosti
	Delay(500);
				
	// Ocitavamo x	
    x_vrednost = sirovi0;
    
	// vode vertikalni tranzistori
    LATCbits.LATC13=0;
    LATCbits.LATC14=1;
	DRIVE_A = 0;  
	DRIVE_B = 1;

    // Cekamo ADC da odradi prevod vrednosti
	Delay(500);
	
	// Ocitavamo y	
    y_vrednost = sirovi1;

    // Skaliranje X-kordinate
    *X = (x_vrednost-161)*0.03629;

    // Skaliranje Y-koordinate
	*Y = ((y_vrednost-500)*0.020725);
}

bool touchscreen_touch() {
    unsigned int X, Y;
    touchscreen_coords(&X, &Y);
    if(X > 128 || Y > 64 || X < 0 || Y < 0)
        return false;
    else return true;
}

bool touchscreen_bound_hit(const bound b, unsigned int X, unsigned int Y) {
    if (!((X>b.x_low)&&(X<b.x_high)&& (Y>b.y_low)&&(Y<b.y_high)))
        return false;
    
    while(touchscreen_touch()) {};
    
    return true;
}
