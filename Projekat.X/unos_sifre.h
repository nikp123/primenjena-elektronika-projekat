/* 
 * File:   unos_sifre.h
 * Author: student
 *
 * Created on 04. decembar 2025., 11.22
 */

#ifndef UNOS_SIFRE_H
#define	UNOS_SIFRE_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#include <stdbool.h>

// cistimo trenutni unos sifre
extern void unos_sifre_clear();

// resetujemo podesenu sifru
extern void unos_sifre_reset();

// setujemo inicijalnu sifru
extern void unos_sifre_set();

// samo stanje
extern bool unos_sifre(void);

// podesiti unos za proveru sifre
extern void unos_sifre_init();

// provera da li je unos tacna sifra
extern bool unos_sifre_is_correct();

#ifdef	__cplusplus
}
#endif

#endif	/* UNOS_SIFRE_H */

