# CMakeLists.txt für SSD1306 I2C Bibliothek
add_library(ssd1306_i2c_lib STATIC
    ssd1306_i2c.c
)

target_include_directories(ssd1306_i2c_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # Für ssd1306_i2c.h
)

# Abhängigkeiten: pico_stdlib und I2C-Treiber
target_link_libraries(ssd1306_i2c_lib
    pico_stdlib
    hardware_i2c
)