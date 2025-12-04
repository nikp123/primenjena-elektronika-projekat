/* 
 * File:   uart.c
 * Author: student
 *
 * Created on 04. decembar 2025., 13.32
 */

#include<p30fxxxx.h>

void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
   // tempRX=U1RXREG;
} 

void uart1_init(void)
{
    U1BRG=0x0040;//ovim odredjujemo baudrate

    U1MODEbits.ALTIO=0;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne

    IEC0bits.U1RXIE=0;//iskljucujemo rx1 interupt

    U1STA&=0xfffc;

    U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul

    U1STAbits.UTXEN=1;//ukljucujemo predaju
}


void uart1_write(unsigned int data)
{
	while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}


/***********************************************************************
* Ime funkcije      : uart1_write_dec2string                     		   *
* Opis              : Funkcija salje 4-cifrene brojeve (cifru po cifru)*
* Parameteri        : unsigned int data-podatak koji zelimo poslati    *
* Povratna vrednost : Nema                                             *
************************************************************************/
void uart1_write_dec2string(unsigned int data)
{
	unsigned char temp;

	temp=data/1000;
	uart1_write(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	uart1_write(temp+'0');
	data=data-temp*100;
	temp=data/10;
	uart1_write(temp+'0');
	data=data-temp*10;
	uart1_write(data+'0');
}
