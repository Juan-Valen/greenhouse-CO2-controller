//
// Created by miro on 10/8/25.
//
#include "FreeRTOS.h"
#include "InfoItem.h"
#include "Menu.h"
#include "PicoOsUart.h"
#include "hardware/gpio.h"
#include "projdefs.h"
#include "rotary.h"
#include "semphr.h"
#include "ssd1306.h"
#include "task.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "hardware/timer.h"

#ifndef RP2040_FREERTOS_IRQ_UI_MAIN_H
#define RP2040_FREERTOS_IRQ_UI_MAIN_H
void rot_callback(uint gpio, uint32_t events);
void rotary_task(void *param);
void rotary_sw_task(void *param);
void display_task(void *param);

#endif //RP2040_FREERTOS_IRQ_UI_MAIN_H
