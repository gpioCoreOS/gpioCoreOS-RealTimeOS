#include "lcd1602_i2c.h"
#include "pico/stdlib.h" // Für sleep_ms
#include "hardware/i2c.h"

// Funktion zum Senden eines Bytes an den PCF8574 (I/O Expander)
static void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(i2c_default, LCD1602_I2C_ADDR, &val, 1, false);
}

// Funktion zum Senden eines Befehls an das LCD
static void lcd_send_cmd(uint8_t cmd) {
    uint8_t data_u, data_l;
    uint8_t data[4];
    data_u = (cmd & 0xF0);
    data_l = ((cmd << 4) & 0xF0);
    data[0] = data_u | 0x0C;  // EN=1, RS=0
    data[1] = data_u | 0x08;  // EN=0, RS=0
    data[2] = data_l | 0x0C;  // EN=1, RS=0
    data[3] = data_l | 0x08;  // EN=0, RS=0
    i2c_write_blocking(i2c_default, LCD1602_I2C_ADDR, data, 4, false);
}

// Funktion zum Senden von Daten an das LCD
static void lcd_send_data(uint8_t data) {
    uint8_t data_u, data_l;
    uint8_t buffer[4];
    data_u = (data & 0xF0);
    data_l = ((data << 4) & 0xF0);
    buffer[0] = data_u | 0x0D;  // EN=1, RS=1
    buffer[1] = data_u | 0x09;  // EN=0, RS=1
    buffer[2] = data_l | 0x0D;  // EN=1, RS=1
    buffer[3] = data_l | 0x09;  // EN=0, RS=1
    i2c_write_blocking(i2c_default, LCD1602_I2C_ADDR, buffer, 4, false);
}

void lcd1602_init(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin) {
    i2c_init(i2c_port, 100 * 1000); // 100kHz I2C
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    sleep_ms(15);
    lcd_send_cmd(0x30); // Wake up
    sleep_ms(5);
    lcd_send_cmd(0x30);
    sleep_us(150);
    lcd_send_cmd(0x30);
    lcd_send_cmd(0x20); // 4-bit mode
    lcd_send_cmd(0x28); // Function set: 2 lines, 5x8 dots
    lcd_send_cmd(0x0C); // Display on, cursor off, blink off
    lcd_send_cmd(0x06); // Entry mode set: increment cursor, no display shift
    lcd1602_clear();
}

void lcd1602_clear() {
    lcd_send_cmd(0x01); // Clear display
    sleep_us(2000);
}

void lcd1602_set_cursor(uint8_t col, uint8_t row) {
    uint8_t address;
    if (row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }
    lcd_send_cmd(address);
}

void lcd1602_print(const char *s) {
    while (*s) {
        lcd_send_data(*s++);
    }
}