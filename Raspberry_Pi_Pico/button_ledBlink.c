#include <stdio.h>
#include "pico/stdlib.h"

#define INPUT_PIN 15   // GPIO pin to read the external signal
#define LED_PIN 25     // Built-in LED pin (GPIO 25)

int main() {
    // Initialize the I/O
    stdio_init_all();

    // Configure the input pin with a pull-down resistor
    gpio_init(INPUT_PIN);
    gpio_set_dir(INPUT_PIN, GPIO_IN);
    gpio_pull_down(INPUT_PIN);

    // Configure the LED pin as output
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        // Read the state of the input pin
        bool input_state = gpio_get(INPUT_PIN);

        if (input_state) {
            // Blink the LED if the input pin is high
            gpio_put(LED_PIN, 1); // Turn on the LED
            sleep_ms(500);        // Wait for 0.5 seconds
            gpio_put(LED_PIN, 0); // Turn off the LED
            sleep_ms(500);        // Wait for 0.5 seconds
        } else {
            // Ensure the LED is off when the input pin is low
            gpio_put(LED_PIN, 0);
        }

        // Small delay to avoid rapid polling
        sleep_ms(100);
    }

    return 0;
}
