# CMakeLists.txt für LCD1602 I2C Bibliothek
add_library(lcd1602_i2c_lib STATIC
    lcd1602_i2c.c
)

target_include_directories(lcd1602_i2c_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # Für lcd1602_i2c.h
)

# Abhängigkeiten: pico_stdlib und I2C-Treiber
target_link_libraries(lcd1602_i2c_lib
    pico_stdlib
    hardware_i2c
)