#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/timer.h"

#define UART_ID uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define LED_PIN 25

volatile int blink_interval_index = 0;
const int blink_intervals[] = {1, 2, 3, 4};  // Different blink intervals in seconds

bool led_blink_callback(repeating_timer_t *rt) {
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
    return true;  // Repeat the timer
}

void uart_receive_callback() {
    while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);
        if (c == 'S') {  // Stop the LED blinking
            cancel_repeating_timer(&rt);
        } else {
            blink_interval_index = (c - '1') % 4;  // Map '1', '2', '3', '4' to 0, 1, 2, 3
            cancel_repeating_timer(&rt);
            add_repeating_timer_ms(blink_intervals[blink_interval_index] * 1000, led_blink_callback, NULL, &rt);
        }
    }
}

int main() {
    stdio_init_all();

    // Initialize UART0
    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Initialize LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Set up UART interrupt
    irq_set_exclusive_handler(UART0_IRQ, uart_receive_callback);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

    // Set initial blink interval
    add_repeating_timer_ms(blink_intervals[blink_interval_index] * 1000, led_blink_callback, NULL, &rt);

    while (true) {
        tight_loop_contents();
    }

    return 0;
}

