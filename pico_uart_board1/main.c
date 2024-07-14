#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void on_uart_irq() {
    while (uart_is_readable(uart0)) {
        char c = uart_getc(uart0);
        uart_putc(UART_ID, c);  // Forward to receiver board
    }
}

int main() {
    stdio_init_all();

    // Initialize UART0 for communication with receiver board
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set up UART interrupt for USB communication
    irq_set_exclusive_handler(UART0_IRQ, on_uart_irq);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

    while (true) {
        tight_loop_contents();  // Enter an idle state, waiting for interrupts
    }

    return 0;
}

