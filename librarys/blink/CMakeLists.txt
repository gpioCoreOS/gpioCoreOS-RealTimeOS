# CMakeLists.txt für Blink Builtin LED Bibliothek
add_library(blink STATIC
    blink.c
)

target_include_directories(blink PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # Für blink_builtin_led.h
)

# Abhängigkeiten: pico_stdlib und GPIO-Treiber
target_link_libraries(blink
    pico_stdlib
    hardware_gpio
)