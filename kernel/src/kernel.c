#include "../include/gpio.h"
#include "../include/uart.h"
#include "../include/timer.h"
#include "../include/ghost_mini_uart.h"

// External assembly functions
extern void enable_irq(void);
extern void disable_irq(void);

void kernel_main(void) {
    // Initialize UART for serial output
    uart_init();
    
    // Clear the screen (send terminal clear command)
    uart_puts("\033[2J\033[H");
    
    // Display boot message with 5-minute delay simulation
    uart_puts("GhostC OS Bootloader v1.0\n");
    uart_puts("Starting boot sequence...\n\n");
    
    const char* boot_stages[] = {
        "Initializing ARM processor...",
        "Loading system modules...",
        "Mounting ghost partitions...",
        "Starting security services...",
        "Initializing Rune environment...",
        "Loading ghost protocols...",
        "Establishing secure connections...",
        "Starting system services...",
        "Performing security checks...",
        "Finalizing boot sequence..."
    };
    
    int num_stages = sizeof(boot_stages) / sizeof(char*);
    
    for (int i = 0; i < num_stages; i++) {
        int percentage = (i * 100) / num_stages;
        
        // Print progress
        uart_puts("\r[");
        for (int j = 0; j < 50; j++) {
            if (j < (percentage / 2)) {
                uart_putc('=');
            } else if (j == (percentage / 2)) {
                uart_putc('>');
            } else {
                uart_putc(' ');
            }
        }
        uart_puts("] ");
        
        // Print percentage
        if (percentage < 10) uart_putc(' ');
        if (percentage < 100) uart_putc(' ');
        uart_puts_int(percentage);
        uart_puts("% ");
        uart_puts(boot_stages[i]);
        uart_puts("\n");
        
        // Delay for about 30 seconds per stage
        for (int j = 0; j < 30; j++) {
            timer_sleep(1000); // Sleep for 1 second
        }
    }
    
    // Clear screen again
    uart_puts("\033[2J\033[H");
    
    // Display ASCII art logo
    uart_puts("\033[32m"); // Green text
    uart_puts("   ▄██████▄     ▄█    █▄     ▄██████▄     ▄████████     ███      ▄████████ \n");
    uart_puts("  ███    ███   ███    ███   ███    ███   ███    ███ ▀█████████▄ ███    ███ \n");
    uart_puts("  ███    █▀    ███    ███   ███    ███   ███    █▀     ▀███▀▀██ ███    █▀  \n");
    uart_puts(" ▄███         ▄███▄▄▄▄███▄▄ ███    ███   ███            ███   ▀ ███        \n");
    uart_puts("▀▀███ ████▄  ▀▀███▀▀▀▀███▀  ███    ███ ▀███████████     ███     ███        \n");
    uart_puts("  ███    ███   ███    ███   ███    ███          ███     ███     ███    █▄  \n");
    uart_puts("  ███    ███   ███    ███   ███    ███    ▄█    ███     ███     ███    ███ \n");
    uart_puts("  ████████▀    ███    █▀     ▀██████▀   ▄████████▀     ▄████▀   ████████▀  \n");
    
    uart_puts("\033[36m"); // Cyan text
    uart_puts("\n                          by: GHOST Sec\n");
    uart_puts("\033[0m"); // Reset text color
    
    uart_puts("\nGhostC OS initialized successfully.\n");
    uart_puts("Type 'help' for available commands.\n\n");
    
    // Initialize system components
    timer_init();
    enable_irq();
    
    // Start command shell
    while (1) {
        uart_puts("ghost@ghostc:~$ ");
        process_command();
    }
}
