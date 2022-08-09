cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(main C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_gpio)

# pico_enable_stdio_usb(${PROJECT_NAME} 1)
# pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})