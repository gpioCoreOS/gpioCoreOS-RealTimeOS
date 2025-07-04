#include "ssd1306_i2c.h"
#include "pico/stdlib.h" // Für sleep_ms
#include "hardware/i2c.h"

// Statische Puffer und globale Variablen für das Display
static uint8_t display_buffer[1024]; // 128x64 / 8 bits per byte

void ssd1306_init(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin) {
    i2c_init(i2c_port, 400 * 1000); // 400kHz I2C
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    // Hier würde die Initialisierungssequenz für das SSD1306 folgen
    // z.B. send_command(SSD1306_DISPLAYOFF); etc.
    // Dies ist eine Vereinfachung
    for (int i = 0; i < sizeof(display_buffer); ++i) {
        display_buffer[i] = 0;
    }
}

void ssd1306_draw_pixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        if (color) {
            display_buffer[x + (y / 8) * 128] |= (1 << (y % 8));
        } else {
            display_buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
        }
    }
}

void ssd1306_refresh() {
    // Befehl zum Setzen des Display-Speicherzeigers auf 0,0
    // ...
    i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, display_buffer, sizeof(display_buffer), false);
}

void ssd1306_clear() {
    for (int i = 0; i < sizeof(display_buffer); ++i) {
        display_buffer[i] = 0;
    }
    ssd1306_refresh();
}