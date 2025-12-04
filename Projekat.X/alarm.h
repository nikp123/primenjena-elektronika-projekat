/* 
 * File:   alarm.h
 * Author: student
 *
 * Created on 04. decembar 2025., 14.41
 */

#ifndef ALARM_H
#define	ALARM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Ovo su svi moguci razlozi zasto bi alarm bio ukljucen
typedef enum razlog {
    ISTEKLO_VREME,  // Isteklo vreme pri unosu sifre (60 sekundi)
    PROVALA,        // Senzor pokreta se upalio tokom zakljucanog stanja, smatra se da je neko provalio u garazu
    GAS,            // Auto je ukljucen u zatvorenoj garazi i proizvodi velike kolicine gasova
    POGRESNA_SIFRA, // Korisnik je pogresio sifru
} razlog;

// Pokreni alarm stanje, dati razlog se koristi kako bi se
// prikazala adekvatna slika korisniku o dogadjaju koji je
// uzrokovao alarm
void alarm_init(razlog r);

// Iskljuci zvucni alarm
void alarm_iskljuci();

// Da li je ukljucen alarm
bool alarm_ukljucen();

// Ponasanje u stanju alarm
bool alarm();

// Ukljuci odbrojavanje za alarm
void alarm_arm();
// Iskljuci odbrojavanje za alarm
void alarm_disarm();
// Da li je ukljuceno odbrojavanje za alarm
bool alarm_is_armed();

#ifdef	__cplusplus
}
#endif

#endif	/* ALARM_H */

