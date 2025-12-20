/* 
 * File:   main.c
 * Author: student
 *
 * Created on 04. decembar 2025., 11.22
 */

#include<p30fxxxx.h>
_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal//
//_FOSC(CSW_ON_FSCM_OFF & HS3_PLL4);
_FWDT(WDT_OFF);
_FGS(CODE_PROT_OFF);

#include <stdio.h>
#include <stdlib.h>

#include "adc.h"
#include "alarm.h"
#include "main.h"
#include "lcd.h"
#include "timer.h"
#include "touchscreen.h"
#include "uart.h"
#include "unos_sifre.h"
#include "otvorena_garaza.h"
#include "zatvorena_garaza.h"

void init() {
    // Namestanje pinova za LCD
	ConfigureLCDPins();
    // Namestanje pinova za touchscreen
	touchscreen_configure_pins();

    // Paljenje LCDa
	GLCD_LcdInit();
    // Brisanje njegovog sadrzaja
	GLCD_ClrScr();
    
    ADCinit();  // Podesavanje ADCa
	touchscreen_configure_pins(); // Podesavanje touchscreen-a
	ADCON1bits.ADON=1; // Ukljucenje ADCa
    
    // Brisemo sifru
    unos_sifre_reset();
    
    // Pokrecemo hardver za UART
    uart1_init();
    
    // A11 je izlazni pin za Buzzer (tj ono sto ce stvarati zvucni alarm)
    TRISA &= ~(1 << 11);
    
    // B7 je izlaz koji ukljucuje nasa svetla u garazi
    TRISB &= ~(1 << 7);
    // B11 je izlaz koji upravlja servo motor koji predstavlja vrata garaze
    TRISB &= ~(1 << 11);    

    // D8 je ulazni pin za sensor gasova
    TRISD |= (1 << 8);
    // D9 je ulazni pin za senzor pokreta
    TRISD |= (1 << 9);

    // Namestanje tajmera
    timer_init_2();
}



/*
 * 
 */
int main(int argc, char** argv) {
    // Podesavamo pocetno stanje
    stanje s = START;

    // Glavna petlja koja regulise prelaze izmedju stanja
    // i njihovu logiku, tkz. "state machine"
    while(1) { switch(s) {
        case START:
            init();
            unos_sifre_clear(); // ciscenje trenutnog unosa
            s = INICIJALNI_UNOS_SIFRE;
            uart1_writeln("Inicijalizacija hardvera uspesna. Podesava se nova sifra.");
            break;
        case INICIJALNI_UNOS_SIFRE:
            if(unos_sifre()) { // Ako je korisnik uneo sifru
                unos_sifre_set(); // Tu sifru pamtimo
                otvorena_garaza_init(); // i otvaramo garazu
                s = GARAZA_OTVORENA;
                uart1_writeln("Sifra podesena. Otvara se garaza");
            }
            break;
        case GARAZA_OTVORENA:
            if(otvorena_garaza()) { // Ako korisnik zeli da zatvara garazu
                zatvorena_garaza_init(); // To uradimo
                s = GARAZA_ZATVORENA;
                uart1_writeln("Zatvara se garaza");
            }
            break;
        case GARAZA_ZATVORENA:
            if(zatvorena_garaza()) { // Ako korisnik zeli da otvori garazu
                unos_sifre_init();   // Pitamo ga za sifru
                s = UNOS_SIFRE;
                uart1_writeln("Korisnik pokusava da udje. Mora sifru da unese");
                uart1_rx_clear(); // Brisemo UART unos u slucaju da se otkljucava preko UARTa
            } else if(PORTDbits.RD9) { // Ako je senzor za pokret bio upaljen
                alarm_init(PROVALA);   // Dizemo alarm za provalu u garazu
                s = ALARM;
                uart1_writeln("Nesto se krece unutar garaze dok je zatvorena. Upali alarm.");
            } else if(!PORTDbits.RD8) { // Ako je CO2 sensor upaljen
                alarm_init(GAS);        // Dizemo alarm za gasove
                s = ALARM;
                uart1_writeln("Gasovi su prisutni u garazi. Upali alarm.");
            } else if(poll_light_sensor()) {
                alarm_init(PROVALA);   // Dizemo alarm za provalu u garazu
                s = ALARM;
                uart1_writeln("Neko svetlo je upaljeno u garazi. Upali alarm.");
            }
            break;
        case UNOS_SIFRE:
            if(unos_sifre() || (uart1_rx_pending() >= 4)) { // Korisnik je zavrsio sa unosom sifre
                if(uart1_rx_pending() >= 4) {
                    // setuj sifru sa uart unosa
                    unos_sifre_override(uart1_rx_peek());
                }
                
                if(unos_sifre_is_correct()) { // Ako je sifra tacna
                    alarm_iskljuci();         // Zvucni alarm se gasi
                    otvorena_garaza_init();   // Otvaraju se vrata garaze
                    s = GARAZA_OTVORENA;
                    uart1_writeln("Ispravna sifra. Otvaraj garazu.");
                } else {
                    alarm_init(POGRESNA_SIFRA); // Pogresna sifra pali alarm za pogresnu sifru
                    s = ALARM;
                    uart1_writeln("Pogresna sifra. Upali alarm.");
                }
            } else if(timer_blew()) { // Da li je isteklo vreme tokom unosa
                alarm_init(ISTEKLO_VREME); // Alarm za isteklo vreme
                s = ALARM;
                uart1_writeln("Isteklo vreme tokom unosa sifre. Upali alarm.");
            }
            /**
             * Ne proveravamo CO2 senzor i senzor za pokret jer mogu nam 
             * blokirati mogucnost unosa sifre ukoliko se ta dva desavanja trenutno
             * odvijaju
             **/
            break;
        case ALARM:
            if(alarm()) { // Ako korisnik zeli da ugasi alarm
                unos_sifre_init(); // Trebace mu unos sifre
                s = UNOS_SIFRE;
                uart1_writeln("Korisnik pokusava da ugasi alarm. Mora da unese sifru.");
                uart1_rx_clear(); // Brisemo UART unos u slucaju da se otkljucava preko UARTa
            }
            break;
    }}
    
    // Ovo NIKAD ne bi trebalo da se izvrsi, ali u slucaju da se to dogodi,
    // resetuje se mikrokontroler
    return (EXIT_SUCCESS);
}

