/*
 * rskrx65n_uart.h
 * --> serial_term_uart.h
 *
 *  Created on: Feb 8, 2018
 *      Author: godocha01
 */

#ifndef VENDOR_CODE_RSKRX65N_UART_H_
#define VENDOR_CODE_RSKRX65N_UART_H_

void uart_config(void);
void uart_string_printf(char * pString);


void uart_printf(char * pString,...);
unsigned short uart_string_scanf(char *pString, unsigned short str_length);
uint16_t uart_input(char *pString, uint16_t maxlen);
uint16_t uart_has_input();
void uart_flush();


void uart_charput(uint32_t output_int);	//TODO Fix wrapper function arguments to char
uint32_t uart_charget (void);			//TODO Fix wrapper function return value to char
 

#endif /* VENDOR_CODE_RSKRX65N_UART_H_ */
