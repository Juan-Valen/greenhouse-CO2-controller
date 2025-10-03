#include "CO2Sensor.h"

int main() {
    CO2Sensor co2(1, 4, 5, 9600, 240); // UART1, TX4/RX5, 9600 bps, Modbus address 240
    co2.startTask(1000); // poll every 1 second

    vTaskStartScheduler(); // start FreeRTOS
    while (1); // should never reach here
}