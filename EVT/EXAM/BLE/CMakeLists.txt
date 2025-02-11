cmake_minimum_required(VERSION 3.20)

add_library(ch592_ble OBJECT)

target_sources(ch592_ble PRIVATE
    LIB/ble_task_scheduler.S
    HAL/MCU.c
    HAL/RTC.c
)
target_include_directories(ch592_ble PUBLIC
    HAL/include
    LIB/
)

target_link_libraries(ch592_ble
    toolchain
    ch592_src
    ${CMAKE_CURRENT_SOURCE_DIR}/LIB/LIBCH59xBLE.a
)
