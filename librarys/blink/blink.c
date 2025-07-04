#include "blink.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void blink_led_init() {
    gpio_init(BUILTIN_LED);
    gpio_set_dir(BUILTIN_LED, GPIO_OUT);
}

void blink_led_once(uint32_t delay_ms) {
    gpio_put(BUILTIN_LED, 1);
    sleep_ms(delay_ms);
    gpio_put(BUILTIN_LED, 0);
    sleep_ms(delay_ms);
}

void blink_led_toggle() {
    static bool led_state = false;
    gpio_put(BUILTIN_LED, led_state);
    led_state = !led_state;
}