/* 
 * File:   touchscreen.h
 * Author: student
 *
 * Created on 04. decembar 2025., 12.38
 */

#ifndef TOUCHSCREEN_H
#define	TOUCHSCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
    
typedef struct bound {
    int8_t x_low, x_high, y_low, y_high;
} bound;

// Namjesti pinove tako da touchscreen radi
void touchscreen_configure_pins(void);

// Preuzmi gole kordinate dodira prista
void touchscreen_coords(unsigned int *X, unsigned int *Y);

// Da li korisnik dodiruje ekran
bool touchscreen_touch();

// Da li je na osnovu datih kordinata, pritisnuto dato ogranicenje "bound"
bool touchscreen_bound_hit(const bound b, unsigned int X, unsigned int Y);

#ifdef	__cplusplus
}
#endif

#endif	/* TOUCHSCREEN_H */

