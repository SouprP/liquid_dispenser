cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(liquid_dispenser C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_executable(liquid_dispenser
    main.cpp
)

# Adding all .hpp and .h files to the project
target_include_directories(liquid_dispenser PUBLIC ./include/)

#################################
# INCLUDE and SRC folder
# Adding .cpp and .c files in /src/
add_library(program_libs
    src/lcd.cpp
    src/hc_sr04.cpp
    src/button.cpp
    src/menu.cpp
    src/pump_driver.cpp
    src/bmp280.cpp
)

target_link_libraries(program_libs
    pico_stdlib 
    pico_multicore
    hardware_i2c
    pico_rand
)

# Ensure program_libs can find Pico SDK headers
target_include_directories(program_libs PUBLIC 
    ./include/
)
#################################

# Pull in common dependencies
target_link_libraries(liquid_dispenser 
    program_libs
    pico_stdlib 
    pico_multicore
    hardware_i2c
    pico_rand
)

pico_enable_stdio_usb(liquid_dispenser 1)
pico_enable_stdio_uart(liquid_dispenser 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(liquid_dispenser)

# Add URL via pico_set_program_url
# example_auto_set_url(liquid_dispenser)