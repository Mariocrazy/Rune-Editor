#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

// System Timer registers for BCM2835
#define TIMER_BASE      0x20003000

#define TIMER_CS        ((volatile uint32_t*)(TIMER_BASE + 0x00))
#define TIMER_CLO       ((volatile uint32_t*)(TIMER_BASE + 0x04))
#define TIMER_CHI       ((volatile uint32_t*)(TIMER_BASE + 0x08))
#define TIMER_C0        ((volatile uint32_t*)(TIMER_BASE + 0x0C))
#define TIMER_C1        ((volatile uint32_t*)(TIMER_BASE + 0x10))
#define TIMER_C2        ((volatile uint32_t*)(TIMER_BASE + 0x14))
#define TIMER_C3        ((volatile uint32_t*)(TIMER_BASE + 0x18))

// Function prototypes
void timer_init(void);
void timer_sleep(uint32_t ms);
uint64_t timer_get_ticks(void);

#endif
