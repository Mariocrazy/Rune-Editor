#ifndef __SYSTEM_H
#define __SYSTEM_H

/* Type definitions */
typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;

/* GDT */
extern void gdt_install(void);
extern void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

/* IDT */
extern void idt_install(void);
extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

/* ISRs */
extern void isrs_install(void);

/* IRQs */
extern void irq_install(void);
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);

/* Timer */
extern void timer_install(void);
extern void timer_wait(int ticks);

/* Keyboard */
extern void keyboard_install(void);
extern char keyboard_getchar(void);

/* Memory Management */
extern void memory_install(void);
extern void *kmalloc(size_t size);
extern void kfree(void *ptr);

/* Paging */
extern void paging_install(void);

/* File System */
extern void fs_init(void);

/* Shell */
extern void shell_init(void);

/* System */
extern void panic(const char *message);
extern void reboot(void);
extern void shutdown(void);

/* Utility */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern char *strcpy(char *dest, const char *src);
extern char *strcat(char *dest, const char *src);

/* Registers structure */
struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

#endif
