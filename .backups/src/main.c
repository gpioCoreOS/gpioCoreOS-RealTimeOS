#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ssd1306_i2c.h"
#include "lcd1602_i2c.h"
#include "blink_builtin_led.h"

// Define I2C pins for SSD1306 and LCD1602 (adjust as needed)
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

// Task für das Blinken der LED
void vBlinkTask(void *pvParameters) {
    blink_led_init();
    for (;;) {
        blink_led_toggle();
        vTaskDelay(pdMS_TO_TICKS(500)); // Blinke alle 500ms
    }
}

// Task für das SSD1306 OLED Display
void vSSD1306Task(void *pvParameters) {
    ssd1306_init(i2c_default, I2C_SDA_PIN, I2C_SCL_PIN);
    ssd1306_clear();
    for (;;) {
        for (int i = 0; i < 128; i += 5) {
            ssd1306_draw_pixel(i, 32, 1); // Ein paar Pixel zeichnen
        }
        ssd1306_refresh();
        vTaskDelay(pdMS_TO_TICKS(2000)); // Alle 2 Sekunden aktualisieren

        ssd1306_clear();
        vTaskDelay(pdMS_TO_TICKS(100)); // Kurze Pause zum Löschen
    }
}

// Task für das LCD1602 Display
void vLCD1602Task(void *pvParameters) {
    lcd1602_init(i2c_default, I2C_SDA_PIN, I2C_SCL_PIN);
    lcd1602_clear();
    lcd1602_set_cursor(0, 0);
    lcd1602_print("Hello Pico RTOS!");
    lcd1602_set_cursor(0, 1);
    lcd1602_print("FreeRTOS rocks!");

    for (;;) {
        // Optional: Text aktualisieren oder Scrollen
        vTaskDelay(pdMS_TO_TICKS(5000)); // Aktualisiere alle 5 Sekunden
    }
}

int main() {
    stdio_init_all();
    printf("Starting FreeRTOS on Pico...\n");

    // Tasks erstellen
    xTaskCreate(vBlinkTask, "BlinkTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vSSD1306Task, "SSD1306Task", configMINIMAL_STACK_SIZE + 256, NULL, 2, NULL); // Mehr Stack für Display
    xTaskCreate(vLCD1602Task, "LCD1602Task", configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL); // Etwas mehr Stack

    // FreeRTOS Scheduler starten
    vTaskStartScheduler();

    // Der Code sollte hier nie ankommen, es sei denn, der Scheduler stoppt.
    for (;;) {
        // Sollte nicht erreicht werden
    }
    return 0;
}