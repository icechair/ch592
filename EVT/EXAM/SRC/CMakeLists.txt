cmake_minimum_required(VERSION 3.20)

add_library(ch592_src OBJECT)

target_sources(ch592_src PRIVATE
    Startup/startup_CH592.S
    RVMSIS/core_riscv.c
    StdPeriphDriver/CH59x_adc.c
    StdPeriphDriver/CH59x_clk.c
    StdPeriphDriver/CH59x_flash.c
    StdPeriphDriver/CH59x_gpio.c
    StdPeriphDriver/CH59x_i2c.c
    StdPeriphDriver/CH59x_lcd.c
    StdPeriphDriver/CH59x_pwm.c
    StdPeriphDriver/CH59x_pwr.c
    StdPeriphDriver/CH59x_spi0.c
    StdPeriphDriver/CH59x_sys.c
    StdPeriphDriver/CH59x_timer0.c
    StdPeriphDriver/CH59x_timer1.c
    StdPeriphDriver/CH59x_timer2.c
    StdPeriphDriver/CH59x_timer3.c
    StdPeriphDriver/CH59x_uart0.c
    StdPeriphDriver/CH59x_uart1.c
    StdPeriphDriver/CH59x_uart2.c
    StdPeriphDriver/CH59x_uart3.c
    StdPeriphDriver/CH59x_usbdev.c
    StdPeriphDriver/CH59x_usbhostBase.c
    StdPeriphDriver/CH59x_usbhostClass.c
)

target_include_directories(ch592_src PUBLIC
    RVMSIS
    StdPeriphDriver/inc
)

target_link_libraries(ch592_src PUBLIC
    toolchain
    ${CMAKE_CURRENT_SOURCE_DIR}/StdPeriphDriver/libISP592.a
)
