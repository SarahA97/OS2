/*
 *
 * Driver for serial I/O
 * uart.c
 *
 * Copyright (C) 2018 Bastiaan Teeuwen <bastiaan@mkcl.nl>
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "mmio.h"

/* 0: QEMU, 1: Pi One/Zero, 2: Pi 2+ */
#define PI_VERS		0

/* Interrupt status registers */
#define INT_OE		0x400
#define INT_BE		0x200
#define INT_PE		0x100
#define INT_FE		0x080
#define INT_RT		0x040
#define INT_TX		0x020
#define INT_RX		0x010
#define INT_DSR		0x008
#define INT_DCD		0x004
#define INT_CTS		0x002
#define INT_RIR		0x001
#define INT_ALLMASK	(INT_OE | INT_BE | INT_PE | INT_FE | INT_RT | INT_TX | \
		INT_RX | INT_DSR | INT_DCD | INT_CTS | INT_RIR)

/* Control register */
#define CR_UART		0x0001
#define CR_RX		0x0200
#define CR_TX		0x0100

/* Line control register */
#define LCR_8BITS	0x60
#define LCR_FIFO	0x10

/* Memory addresses */
#if PI_VERS == 0
#  define PERH_OFFSET	0x0FFF0000
#elif PI_VERS == 1
#  define PERH_OFFSET	0x20000000
#else
#  define PERH_OFFSET	0x3F000000
#endif

#define GPIO_OFFSET	(PERH_OFFSET + 0x200000)
#define UART0_OFFSET	(PERH_OFFSET + 0x201000)

/* GPIO pull-up/down registers */
#define GPPUD		(GPIO_OFFSET + 0x94)
#define GPPUD_CLK0	(GPIO_OFFSET + 0x98)

/* UART 0 registers */
#define UART0_DR	(UART0_OFFSET + 0x00)
#define UART0_RSRECR	(UART0_OFFSET + 0x04)
#define UART0_FR	(UART0_OFFSET + 0x18)
#define UART0_ILPR	(UART0_OFFSET + 0x20)
#define UART0_IBRD	(UART0_OFFSET + 0x24)
#define UART0_FBRD	(UART0_OFFSET + 0x28)
#define UART0_LCRH	(UART0_OFFSET + 0x2C)
#define UART0_CR	(UART0_OFFSET + 0x30)
#define UART0_IFLS	(UART0_OFFSET + 0x34)
#define UART0_IMSC	(UART0_OFFSET + 0x38)
#define UART0_RIS	(UART0_OFFSET + 0x3C)
#define UART0_MIS	(UART0_OFFSET + 0x40)
#define UART0_ICR	(UART0_OFFSET + 0x44)
#define UART0_DMACR	(UART0_OFFSET + 0x48)
#define UART0_ITCR	(UART0_OFFSET + 0x80)
#define UART0_ITIP	(UART0_OFFSET + 0x84)
#define UART0_ITOP	(UART0_OFFSET + 0x88)
#define UART0_TDR	(UART0_OFFSET + 0x8C)

static inline void delay(int32_t count)
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
			: "=r"(count): [count]"0"(count) : "cc");
}

void uart_putc(char c)
{
	/* Friggin' serial console */
	if (c == '\n')
		uart_putc('\r');

	while (mmio_read(UART0_FR) & (1 << 5));

	mmio_write(UART0_DR, c);
}

char uart_getc(void)
{
	while (mmio_read(UART0_FR) & (1 << 4));

	return mmio_read(UART0_DR);
}

void uart_puts(const char *str)
{
	while (*str)
		uart_putc(*str++);
}

void uart_init(void)
{
	/* Disable UART 0 */
	mmio_write(UART0_CR, 0);

	/* Enable UART0 GPIO on pins 14 & 15 */
	mmio_write(GPPUD, 0);
	delay(150);

	mmio_write(GPPUD_CLK0, (1 << 14) | (1 << 15));
	delay(150);

	mmio_write(GPPUD_CLK0, 0);

	/* Clear all interrupts */
	mmio_write(UART0_ICR, INT_ALLMASK);

	/* Set baud rate to 115200 */
	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);

	/* Setup line control to 8 bits per word */
	mmio_write(UART0_LCRH, LCR_FIFO | LCR_8BITS);

	/* Mask all interrupts */
	mmio_write(UART0_IMSC, INT_ALLMASK);

	/* Enable TX and RX on UART 0 */
	mmio_write(UART0_CR, CR_UART | CR_TX | CR_RX);
}
