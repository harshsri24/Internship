#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define INPUT_PIN 15   // GPIO pin to read the external signal
#define LED_PIN 25     // Built-in LED pin (GPIO 25)

bool blink = false;
uint32_t blink_interval_ms = 1000;  // Initial blink interval in milliseconds (1 second)

// GPIO interrupt callback function
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == INPUT_PIN) {
        blink = gpio_get(INPUT_PIN);
        if (!blink) {
            gpio_put(LED_PIN, 0);  
        } else {
              if (blink_interval_ms == 1000) {
                blink_interval_ms = 2000;  // Change to 2 seconds if it was 1 second
            } else {
                blink_interval_ms = 1000;  // Change back to 1 second if it was 2 seconds
            }
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
    absolute_time_t last_toggle_time = get_absolute_time();

    while (true) {
        if (blink) {
            // Get the current time
            absolute_time_t current_time = get_absolute_time();

            // Calculate time elapsed since last toggle
            uint32_t elapsed_ms = absolute_time_diff_us(last_toggle_time, current_time) / 1000;

            // Check if enough time has elapsed for the current blink interval
            if (elapsed_ms >= blink_interval_ms) {
                gpio_put(LED_PIN, !gpio_get(LED_PIN));

                // Update the last toggle time
                last_toggle_time = current_time;
            }
        } else {
            gpio_put(LED_PIN, 0);
        }

        // Small delay to avoid rapid polling
        sleep_ms(10);
    }

    return 0;
}
