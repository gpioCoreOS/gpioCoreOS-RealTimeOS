# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 12_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(whiteCoreOS C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

set(PICO_EXTRAS_PATH ${PICO_SDK_PATH}/../pico-extras)
set(PICO_PLAYGROUND_PATH ${PICO_SDK_PATH}/../pico-playground)
set(FREERTOS_KERNEL_PATH ${PICO_SDK_PATH}/../freertos/FreeRTOS-Kernel) # <-- Achte auf diesen Pfad!

# Optional: Setzen Sie PICO_BOARD, wenn Sie ein spezifisches Board verwenden
# set(PICO_BOARD pico_w) # Beispiel fÃ¼r Pico W

# FÃ¼ge pico-extras und pico-playground hinzu
add_subdirectory(${PICO_EXTRAS_PATH} pico_extras)
add_subdirectory(${PICO_PLAYGROUND_PATH} pico_playground)

# FÃ¼ge FreeRTOS-Kernel hinzu
# Dies ist WICHTIG, damit der FreeRTOS-Kernel selbst kompiliert wird!
add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)


add_executable(whiteCoreOS whiteCoreOS.c )

pico_set_program_name(whiteCoreOS "whiteCoreOS")
pico_set_program_version(whiteCoreOS "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(whiteCoreOS 0)
pico_enable_stdio_usb(whiteCoreOS 1)
# VerknÃ¼pfung des Linker-Skripts fÃ¼r FreeRTOS (empfohlen)
pico_set_linker_script(${CMAKE_PROJECT_NAME} ${PICO_SDK_PATH}/src/rp2040/pico_rp2040_inkernel.ld)


target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/whiteCoreOS.c # Quelle fÃ¼r das Hauptprogramm
)

# HinzufÃ¼gen der Bibliotheksverzeichnisse
# Dies fÃ¼gt die Unterverzeichnisse hinzu, in denen Ihre Treiber-CMakeLists.txt-Dateien liegen
add_subdirectory(librarys/ssd1306_i2c_lib)
add_subdirectory(librarys/lcd1602_i2c_lib)
add_subdirectory(librarys/blink) # <-- Beachten Sie den Ordnernamen 'blink'

# Add the standard library to the build
target_link_libraries(whiteCoreOS
        pico_stdlib
        FreeRTOS-Kernel # Stellt sicher, dass die FreeRTOS-Kernel-Bibliothek verknÃ¼pft wird
        pico_extras # VerknÃ¼pft pico-extras Bibliotheken
        pico_playground # VerknÃ¼pft pico-playground Bibliotheken
        ssd1306_i2c_lib # Name der Bibliothek aus librarys/ssd1306_i2c_lib/CMakeLists.txt
        lcd1602_i2c_lib # Name der Bibliothek aus librarys/lcd1602_i2c_lib/CMakeLists.txt
        blink_builtin_led_lib # <-- KORREKTUR: Verwenden Sie den Library-Namen aus der CMakeLists.txt der Bibliothek
)

## FreeRTOS Konfiguration Includes
# Hier ist es wichtig, die FreeRTOS-Konfigurationsdatei (FreeRTOSConfig.h) anzugeben.
# Sie sollten eine eigene FreeRTOSConfig.h in Ihrem Projektverzeichnis haben.
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # <-- KORREKTUR: FÃ¼r whiteCoreOS.c und FreeRTOSConfig.h im Hauptverzeichnis
    ${FREERTOS_KERNEL_PATH}/include # FreeRTOS Kernel includes
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0 # Oder den spezifischen Port, den Sie verwenden
)

# Entfernt, da es redundant ist und die obige Zeile den Zweck erfÃ¼llt.
# target_include_directories(whiteCoreOS PRIVATE
#         ${CMAKE_CURRENT_LIST_DIR}
# )

# Add any user requested libraries
target_link_libraries(whiteCoreOS
        hardware_spi
        hardware_i2c
        hardware_timer
)

pico_add_extra_outputs(whiteCoreOS)


# Optional: Debugging Einstellungen
# pico_enable_gdbstub(${CMAKE_PROJECT_NAME} 1)

# Hier kÃ¶nnen Sie weitere Definitionen hinzufÃ¼gen, z.B. fÃ¼r FreeRTOS oder Ihre Treiber
# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
#     MY_DRIVER_DEFINE=1
# )