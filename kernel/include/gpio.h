#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>

// GPIO registers base address for BCM2835 (Pi Zero W)
#define GPIO_BASE       0x20200000

// GPIO function select registers
#define GPFSEL0         ((volatile uint32_t*)(GPIO_BASE + 0x00))
#define GPFSEL1         ((volatile uint32_t*)(GPIO_BASE + 0x04))
#define GPFSEL2         ((volatile uint32_t*)(GPIO_BASE + 0x08))
#define GPFSEL3         ((volatile uint32_t*)(GPIO_BASE + 0x0C))
#define GPFSEL4         ((volatile uint32_t*)(GPIO_BASE + 0x10))
#define GPFSEL5         ((volatile uint32_t*)(GPIO_BASE + 0x14))

// GPIO output set registers
#define GPSET0          ((volatile uint32_t*)(GPIO_BASE + 0x1C))
#define GPSET1          ((volatile uint32_t*)(GPIO_BASE + 0x20))

// GPIO output clear registers
#define GPCLR0          ((volatile uint32_t*)(GPIO_BASE + 0x28))
#define GPCLR1          ((volatile uint32_t*)(GPIO_BASE + 0x2C))

// GPIO level registers
#define GPLEV0          ((volatile uint32_t*)(GPIO_BASE + 0x34))
#define GPLEV1          ((volatile uint32_t*)(GPIO_BASE + 0x38))

// GPIO pull-up/down registers
#define GPPUD           ((volatile uint32_t*)(GPIO_BASE + 0x94))
#define GPPUDCLK0       ((volatile uint32_t*)(GPIO_BASE + 0x98))
#define GPPUDCLK1       ((volatile uint32_t*)(GPIO_BASE + 0x9C))

// GPIO function settings
#define GPIO_INPUT      0x0
#define GPIO_OUTPUT     0x1
#define GPIO_ALT0       0x4
#define GPIO_ALT1       0x5
#define GPIO_ALT2       0x6
#define GPIO_ALT3       0x7
#define GPIO_ALT4       0x3
#define GPIO_ALT5       0x2

// Function prototypes
void gpio_init(void);
void gpio_set_function(int pin, int function);
void gpio_set(int pin);
void gpio_clear(int pin);
int gpio_get(int pin);
void gpio_set_pull(int pin, int pull);

#endif
