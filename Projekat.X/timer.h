// -*- C++ -*-
/* 
 * File:   Tajmeri.h
 * Author: Milan
 *
 * Created on November 8, 2017, 10:48 PM
 */

#ifndef TIMER_H
#define TIMER_H

#include <p30fxxxx.h>

#include <stdbool.h>

extern bool stepper_engaged;

// Resetuj timer za unos
void timer_reset();

// Da li je istekao timer za unos
bool timer_blew();

// Daj trenutno vreme u milisekundama
// (Korisno zarad dobijanja periode za upravljanje servo motora)
unsigned int timer_ms();

// Inicijalizacija timera 2
void timer_init_2(void);

// delay funkcija, vreme nije precizno, blokira kretanje koda
void Delay(unsigned int N);

#endif
