/* 
 * File:   lcd.c
 * Author: student
 *
 * Created on 04. decembar 2025., 12.32
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "driverGLCD.h"

void draw(const uint8_t picture[]) {
    uint8_t vert, q;
    for (vert = 0; vert< 8; vert++) {
		GoToXY(0,   vert);
		for (q = 0; q < 64; q++) {
			LcdDataWrite(picture[vert * 128 + q]);
		}
		GoToXY(64,  vert);
		for (q = 0; q < 64; q++) {
            LcdDataWrite(picture[vert * 128 + q + 64]);
		}
	}
}