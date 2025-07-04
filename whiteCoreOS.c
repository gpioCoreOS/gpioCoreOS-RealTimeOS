#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ssd1306_i2c.h"
#include "lcd1602_i2c.h"
#include "blink.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9


// Define I2C pins for SSD1306 and LCD1602 (adjust as needed)
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5


int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5



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



int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    stdio_init_all();
    printf("Starting FreeRTOS on Pico...\n");

    // Tasks erstellen
    xTaskCreate(vBlinkTask, "BlinkTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vSSD1306Task, "SSD1306Task", configMINIMAL_STACK_SIZE + 256, NULL, 2, NULL); // Mehr Stack für Display
    xTaskCreate(vLCD1602Task, "LCD1602Task", configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL); // Etwas mehr Stack

    // FreeRTOS Scheduler starten
    vTaskStartScheduler();

    

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }

    // Der Code sollte hier nie ankommen, es sei denn, der Scheduler stoppt.
    for (;;) {
        // Sollte nicht erreicht werden
    }
    return 0;
}




