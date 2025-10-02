//
// Created by Keijo Länsikunnas on 10.9.2024.
//

#ifndef RP2040_FREERTOS_IRQ_BLINKER_H
#define RP2040_FREERTOS_IRQ_BLINKER_H
#include <string>
#include "pico/stdlib.h" // IWYU pragma: keep
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

class Blinker {
public:
    Blinker(int led_);
    void off();
    void on(uint32_t delay);
private:
    void run();
    static void runner(void *params);
    const std::string name;
    int led;
    int delay;
    TaskHandle_t handle;
};

#endif //RP2040_FREERTOS_IRQ_BLINKER_H
