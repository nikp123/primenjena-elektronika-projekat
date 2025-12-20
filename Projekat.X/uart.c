/* 
 * File:   uart.c
 * Author: student
 *
 * Created on 04. decembar 2025., 13.32
 */

#include<p30fxxxx.h>
#include <stdbool.h>

int  rx_size = 0;
char rx_input[100];
bool rx_done = false;

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    if(!rx_done) {
        char newchar = U1RXREG;
        // sacuvaj novi karakter
        rx_input[rx_size++] = newchar;
        
        // u slucaju da neko hoce da provali memoriju
        if(rx_size == 99) {
            rx_done = true;
        }
        
        // u slucaju da nadolazi nova linija
        if(newchar == '\n')
            rx_done = true;
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) 
{
    IFS0bits.U1TXIF = 0;
    U1RXREG = 'A';
} 

void uart1_rx_clear(void) {
    rx_input[0] = '0';
    rx_done = false;
    rx_size = 0;
}

unsigned char uart1_rx_pending(void) {
    return rx_size;
}

char *uart1_rx_peek(void) {
    // Za bezbednost u slucaju da kod istica ulaz kao string
    rx_input[rx_size] = '\0';
    return rx_input;
}

void uart1_init(void)
{
    // 9600 baud
    // 10_000_000 / (16 * (0x0040 + 1))
    U1BRG             = 0x0040; // ovim odredjujemo baudrate
    U1MODEbits.ALTIO  = 0; // biramo koje pinove koristimo za komunikaciju osnovne ili alternativne
    IEC0bits.U1RXIE   = 1; // omogucavamo rx1 interupt

    U1STA             &= 0xfffc;
    U1MODEbits.UARTEN = 1;  // ukljucujemo ovaj modul
    U1STAbits.UTXEN   = 1;  // ukljucujemo predaju
    
    /* wait at least 104 usec (1/9600) before sending first char */
    int i = 0;
    for(i = 0; i < 4160; i++){
        Nop();
    }
}


void uart1_write(unsigned int data)
{
	while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void uart1_writeln(char *data)
{
	while(*data) {
        uart1_write(*data++);
    }
    uart1_write('\n');
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
