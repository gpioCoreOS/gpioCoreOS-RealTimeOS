#ifndef SSD1306_I2C_H
#define SSD1306_I2C_H

#include <stdint.h>
#include "hardware/i2c.h"

#define SSD1306_I2C_ADDR 0x3C // Oder 0x3D, je nach Modul

void ssd1306_init(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin);
void ssd1306_draw_pixel(int x, int y, uint8_t color);
void ssd1306_refresh();
void ssd1306_clear();

#endif // SSD1306_I2C_H