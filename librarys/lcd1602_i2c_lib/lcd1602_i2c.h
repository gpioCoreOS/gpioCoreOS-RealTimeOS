#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include <stdint.h>
#include "hardware/i2c.h"

#define LCD1602_I2C_ADDR 0x27 // Häufigste Adresse, kann 0x3F sein

void lcd1602_init(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin);
void lcd1602_clear();
void lcd1602_set_cursor(uint8_t col, uint8_t row);
void lcd1602_print(const char *s);

#endif // LCD1602_I2C_H