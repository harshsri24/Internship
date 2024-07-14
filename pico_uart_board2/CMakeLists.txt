cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/pico_sdk_import.cmake)

project(pico_uart_board2)

pico_sdk_init()

add_executable(pico_uart_board2
    main.c
)

target_link_libraries(pico_uart_board2 pico_stdlib hardware_uart hardware_timer)

pico_add_extra_outputs(pico_uart_board2)

