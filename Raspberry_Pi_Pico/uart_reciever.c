#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define UART_ID uart0
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define LED_PIN 25     

volatile int blink_interval_ms = 1000;  
volatile bool led_state = false;
struct repeating_timer timer;

bool led_blink_callback(struct repeating_timer *t) {
    gpio_put(LED_PIN, led_state);
    led_state = !led_state;
    return true;
}

void uart_receive_callback() {
    while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);
        switch (c) {
            case '1':
                blink_interval_ms = 1000;
                break;
            case '2':
                blink_interval_ms = 2000;
                break;
            case '3':
                blink_interval_ms = 3000;
                break;
            case '4':
                blink_interval_ms = 4000;
                break;
        }
        cancel_repeating_timer(&timer);
        add_repeating_timer_ms(blink_interval_ms, led_blink_callback, NULL, &timer);
    }
}

void on_uart_irq() {
    uart_receive_callback();
}

int main() {
    stdio_init_all();

    uart_init(UART_ID, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    irq_set_exclusive_handler(UART0_IRQ, on_uart_irq);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    add_repeating_timer_ms(blink_interval_ms, led_blink_callback, NULL, &timer);

    while (true) {
        tight_loop_contents();  // Enter an idle state, waiting for interrupts
    }

    return 0;
}
