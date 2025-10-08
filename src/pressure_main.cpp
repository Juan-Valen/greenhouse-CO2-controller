#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "PressureSensor.h"
#include <stdio.h>


static PressureSensor pressure;

void vPressureTask(void *pvParameters) {
    PressureSensor *sensor = static_cast<PressureSensor*>(pvParameters);
    while (true) {
        float pa = sensor->readPressurePa();
        printf("Pressure: %.2f Pa\n", pa);
        vTaskDelay(pdMS_TO_TICKS(1000)); // every 1s
    }
}

int main() {
    stdio_init_all();

    // Initialize the pressure sensor (I2C1, pins 14/15 @ 100kHz)
    pressure.begin(14, 15, 100000);

    // Create the FreeRTOS task
    xTaskCreate(vPressureTask, "PressureTask", 1024, &pressure, 1, NULL);

    vTaskStartScheduler();

    while (true) {
        // Should never get here
    }
}
