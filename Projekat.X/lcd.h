/* 
 * File:   lcd.h
 * Author: student
 *
 * Created on 04. decembar 2025., 12.33
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "driverGLCD.h"

#include <stdint.h>

// funkcija koja na GLCDu nacrta zeljenu sliku
extern void draw(const uint8_t picture[]);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

