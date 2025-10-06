#include <pico/stdio.h>
#include "CO2Sensor.h"
#include <iostream>
#include <sstream>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hardware/gpio.h"
#include "PicoOsUart.h"
#include "ssd1306.h"
#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}
}
int main() {
    stdio_init_all();
    CO2Sensor co2(1, 4, 5, 9600, 240); // UART1, TX4/RX5, 9600 bps, Modbus address 240
    co2.startTask(1000); // poll every 1 second

    vTaskStartScheduler(); // start FreeRTOS
    while (1); // should never reach here
}