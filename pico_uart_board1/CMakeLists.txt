cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(pico_uart_board1)

pico_sdk_init()

add_executable(pico_uart_board1
    main.c
)

target_link_libraries(pico_uart_board1 pico_stdlib hardware_uart)

pico_add_extra_outputs(pico_uart_board1)

