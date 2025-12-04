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
void uart1_write_dec2string(unsigned int data);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

