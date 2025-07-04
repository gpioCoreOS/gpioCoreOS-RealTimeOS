#ifndef BLINK_BUILTIN_LED_H
#define BLINK_BUILTIN_LED_H

#include "pico/stdlib.h"

#ifdef PICO_DEFAULT_LED_PIN
#define BUILTIN_LED PICO_DEFAULT_LED_PIN
#else
// Definiere einen Standard-LED-Pin, falls PICO_DEFAULT_LED_PIN nicht definiert ist
#define BUILTIN_LED 25
#endif

void blink_led_init();
void blink_led_once(uint32_t delay_ms);
void blink_led_toggle();

#endif // BLINK_BUILTIN_LED_H