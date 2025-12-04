/* 
 * File:   main.h
 * Author: student
 *
 * Created on 04. decembar 2025., 11.49
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif


typedef enum stanje {
    START,
    INICIJALNI_UNOS_SIFRE,
    GARAZA_OTVORENA,
    GARAZA_ZATVORENA,
    UNOS_SIFRE,
    ALARM
} stanje;

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

