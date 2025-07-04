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
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
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
set(FREERTOS_KERNEL_PATH ${PICO_SDK_PATH}/../freertos/FreeRTOS-Kernel) # Oder wo auch immer Sie FreeRTOS geklont haben

# Optional: Setzen Sie PICO_BOARD, wenn Sie ein spezifisches Board verwenden
# set(PICO_BOARD pico_w) # Beispiel für Pico W

# Füge pico-extras und pico-playground hinzu
add_subdirectory(${PICO_EXTRAS_PATH} pico_extras)
add_subdirectory(${PICO_PLAYGROUND_PATH} pico_playground)

add_executable(whiteCoreOS whiteCoreOS.c )

pico_set_program_name(whiteCoreOS "whiteCoreOS")
pico_set_program_version(whiteCoreOS "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(whiteCoreOS 0)
pico_enable_stdio_usb(whiteCoreOS 1)
pico_set_linker_script(${CMAKE_PROJECT_NAME} ${PICO_SDK_PATH}/src/rp2040/pico_rp2040_inkernel.ld) # Wichtig für FreeRTOS


target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/whiteCoreOS.c
)

# Hinzufügen der Bibliotheksverzeichnisse
# Dies fügt die Unterverzeichnisse hinzu, in denen Ihre Treiber-CMakeLists.txt-Dateien liegen
add_subdirectory(librarys/ssd1306_i2c_lib)
add_subdirectory(librarys/lcd1602_i2c_lib)
add_subdirectory(librarys/blink)

# Add the standard library to the build
target_link_libraries(whiteCoreOS
        pico_stdlib
        FreeRTOS-Kernel # Stellt sicher, dass die FreeRTOS-Kernel-Bibliothek verknüpft wird
        pico_extras # Verknüpft pico-extras Bibliotheken
        pico_playground # Verknüpft pico-playground Bibliotheken
        ssd1306_i2c_lib # Name der Bibliothek aus lib/ssd1306_i2c/CMakeLists.txt
        lcd1602_i2c_lib # Name der Bibliothek aus lib/lcd1602_i2c/CMakeLists.txt
        blink # 
)
## FreeRTOS Konfiguration
# Hier ist es wichtig, die FreeRTOS-Konfigurationsdatei (FreeRTOSConfig.h) anzugeben.
# Sie sollten eine eigene FreeRTOSConfig.h in Ihrem Projektverzeichnis haben.
# Beispiel: src/FreeRTOSConfig.h
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # Für main.c und FreeRTOSConfig.h
    ${FREERTOS_KERNEL_PATH}/include # FreeRTOS Kernel includes
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0 # Oder den spezifischen Port, den Sie verwenden
)
#

# Add the standard include files to the build
target_include_directories(whiteCoreOS PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(whiteCoreOS 
        hardware_spi
        hardware_i2c
        hardware_timer
        )

pico_add_extra_outputs(whiteCoreOS)


# Optional: Debugging Einstellungen
# pico_enable_gdbstub(${CMAKE_PROJECT_NAME} 1)

# Hier können Sie weitere Definitionen hinzufügen, z.B. für FreeRTOS oder Ihre Treiber
# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
#     MY_DRIVER_DEFINE=1
# )