/*
 *
 * Driver for serial I/O header
 * uart.h
 *
 * Copyright (C) 2018 Bastiaan Teeuwen <bastiaan@mkcl.nl>
 *
 */

#ifndef _UART_H
#define _UART_H

void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *str);
void uart_init(void);

#endif
