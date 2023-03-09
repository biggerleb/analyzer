cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(analyzer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_subdirectory(lib/config)
add_subdirectory(lib/lcd)
add_subdirectory(lib/font)
add_subdirectory(src)
add_subdirectory(lib/fatfs)
add_subdirectory(lib/sdcard)

include_directories(src)
include_directories(./lib/config)
include_directories(./lib/lcd)
include_directories(./lib/font)
include_directories(./lib/fatfs)
include_directories(./lib/sdcard)

add_executable(main 
main.cpp
)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)

target_link_libraries(main src lcd font config pico_stdlib hardware_i2c hardware_spi fatfs)
