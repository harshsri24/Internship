#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define INPUT_PIN 15   
#define LED_PIN 25     

bool blink = false;
uint32_t blink_intervals[] = {1000, 2000, 3000, 4000};  // Blink intervals in milliseconds
int current_interval_index = 0;  // Index of the current blink interval

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == INPUT_PIN) {
        blink = gpio_get(INPUT_PIN);
        if (!blink) {
            gpio_put(LED_PIN, 0);  
        } else {
            // Cycle through different blink intervals
            current_interval_index = (current_interval_index + 1) % 4;
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

    // Get the current time for initial LED toggle
    absolute_time_t last_toggle_time = get_absolute_time();

    while (true) {
        if (blink) {
            absolute_time_t current_time = get_absolute_time();

            // Calculate time elapsed since last toggle
            uint32_t elapsed_ms = absolute_time_diff_us(last_toggle_time, current_time) / 1000;

            // Check if enough time has elapsed for the current blink interval
            if (elapsed_ms >= blink_intervals[current_interval_index]) {
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
