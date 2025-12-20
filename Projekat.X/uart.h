/* 
 * File:   uart.h
 * Author: student
 *
 * Created on 04. decembar 2025., 13.35
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

void uart1_init(void);
void uart1_write(unsigned int data);
void uart1_writeln(char *data);
void uart1_write_dec2string(unsigned int data);

void uart1_rx_clear(void);
unsigned char uart1_rx_pending(void);
char *uart1_rx_peek(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

