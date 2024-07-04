#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define SIGNAL_PIN 15   

volatile int blink_interval_index = 0;
const char* blink_intervals[] = {"1", "2", "3", "4"};  // Different blink intervals

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == SIGNAL_PIN) {
        uart_puts(UART_ID, blink_intervals[blink_interval_index]);
        blink_interval_index = (blink_interval_index + 1) % 4;  
    }
}

int main() {
    stdio_init_all();

    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    gpio_init(SIGNAL_PIN);
    gpio_set_dir(SIGNAL_PIN, GPIO_IN);
    gpio_pull_down(SIGNAL_PIN);  // Default state is low

    gpio_set_irq_enabled_with_callback(SIGNAL_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (true) {
        tight_loop_contents();  
    }

    return 0;
}
