#ifndef __GHOST_VGA_H
#define __GHOST_VGA_H

#include "system.h"

/* VGA Colors */
enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE,
};

/* VGA Functions */
extern void clear_screen(void);
extern void kprint(const char *str);
extern void kprint_color(const char *str, uint8_t color);
extern void set_text_color(uint8_t foreground, uint8_t background);
extern void init_video(void);

/* Screen dimensions */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#endif
