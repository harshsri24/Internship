#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define INPUT_PIN 15   // GPIO pin to read the external signal
#define LED_PIN 25     // Built-in LED pin (GPIO 25)

volatile bool led_on = false;
volatile bool blink = false;
struct repeating_timer timer;

bool timer_callback(struct repeating_timer *t) {
    if (blink) {
        gpio_put(LED_PIN, led_on);
        led_on = !led_on;
    }
    return true;
}

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == INPUT_PIN) {
        blink = gpio_get(INPUT_PIN);
        if (!blink) {
            gpio_put(LED_PIN, 0);  // Turn off LED if not blinking
        }
    }
}

int main() {
    stdio_init_all();
    gpio_init(INPUT_PIN);
    gpio_set_dir(INPUT_PIN, GPIO_IN);
    gpio_pull_down(INPUT_PIN);
    gpio_set_irq_enabled_with_callback(INPUT_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    add_repeating_timer_ms(500, timer_callback, NULL, &timer);

    while (true) {
        tight_loop_contents();
    }

    return 0;
}
