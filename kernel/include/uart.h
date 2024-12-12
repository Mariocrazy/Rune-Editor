#ifndef __UART_H
#define __UART_H

#include <stdint.h>

// UART registers for BCM2835
#define UART0_BASE      0x20201000

// UART registers
#define UART0_DR        ((volatile uint32_t*)(UART0_BASE + 0x00))
#define UART0_FR        ((volatile uint32_t*)(UART0_BASE + 0x18))
#define UART0_IBRD      ((volatile uint32_t*)(UART0_BASE + 0x24))
#define UART0_FBRD      ((volatile uint32_t*)(UART0_BASE + 0x28))
#define UART0_LCRH      ((volatile uint32_t*)(UART0_BASE + 0x2C))
#define UART0_CR        ((volatile uint32_t*)(UART0_BASE + 0x30))
#define UART0_ICR       ((volatile uint32_t*)(UART0_BASE + 0x44))

// Function prototypes
void uart_init(void);
void uart_putc(unsigned char c);
void uart_puts(const char* str);
void uart_puts_int(int num);
unsigned char uart_getc(void);
void uart_clear(void);
void process_command(void);

// UART settings
#define UART_CLOCK      48000000
#define UART_BAUD       115200

#endif
