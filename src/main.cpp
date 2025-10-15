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
#include <CO2Sensor.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "hardware/timer.h"
extern "C" {
uint32_t read_runtime_ctr(void) { return timer_hw->timerawl; }
}

#include "blinker.h"

SemaphoreHandle_t gpio_sem_rot;
SemaphoreHandle_t gpio_sem_rot_sw;

void rot_callback(uint gpio, uint32_t events) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  static uint32_t last_time = 0;
  if (gpio == 10) {
    xSemaphoreGiveFromISR(gpio_sem_rot, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  } else if (gpio == 12) {
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_time > 250) {
      last_time = now;
      xSemaphoreGiveFromISR(gpio_sem_rot_sw, &xHigherPriorityTaskWoken);
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
  }
}

struct rot_params {
  QueueHandle_t comm_rot;
};

struct rot_sw_params {
  QueueHandle_t comm_sw;
};

struct display_params {
  QueueHandle_t comm_rot;
  QueueHandle_t comm_sw;
  QueueHandle_t sensor_1_queue;

QueueHandle_t comm_temp;
QueueHandle_t comm_hum;
/*QueueHandle_t comm_pres;
   */
};

void rotary_task(void *param) {
  auto tpr = (rot_params *)param;
  // Rotary encoder
  static Rotary rot(10, 11, 12);
  bool clockwise;
  // IRQ
  gpio_set_irq_enabled_with_callback(10, GPIO_IRQ_EDGE_RISE, true,
                                     &rot_callback);
  while (true) {
    if (xSemaphoreTake(gpio_sem_rot, portMAX_DELAY) == pdTRUE) {
      clockwise = rot.rotatingClockwise();
      xQueueSendToBack(tpr->comm_rot, (void *)&clockwise, 0);
    }
  }
}

void rotary_sw_task(void *param) {
  static bool value = true;
  auto tpr = (rot_sw_params *)param;
  // IRQ
  gpio_set_irq_enabled_with_callback(12, GPIO_IRQ_EDGE_FALL, true,
                                     &rot_callback);
  while (true) {
    if (xSemaphoreTake(gpio_sem_rot_sw, portMAX_DELAY) == pdTRUE) {
      xQueueSendToBack(tpr->comm_sw, (void *)&value, 0);
    }
  }
}

void display_task(void *param);

int main() {
  stdio_init_all();
  printf("\nBoot\n");

  // Parameters
  static rot_params rt0 = {};
  static rot_sw_params rt_sw = {};
  static display_params dp0 = {};
  // Queue
  rt0.comm_rot = xQueueCreate(20, sizeof(bool));
  rt_sw.comm_sw = xQueueCreate(5, sizeof(bool));
  auto sensor_1_queue = xQueueCreate(5, sizeof(bool));
  dp0.comm_rot = rt0.comm_rot;
  dp0.comm_sw = rt_sw.comm_sw;
  dp0.sensor_1_queue = sensor_1_queue;
  // Semaphores
  gpio_sem_rot = xSemaphoreCreateBinary();
  gpio_sem_rot_sw = xSemaphoreCreateBinary();
  // CO2

  CO2Sensor co2(1, 4, 5, 9600,
                240);            // UART1, TX4/RX5, 9600 bps, Modbus address 240
  co2.startTask(sensor_1_queue); // poll every 1 second
  // TASKS
  xTaskCreate(rotary_task, "rotary_encoder", 256, (void *)&rt0,
              tskIDLE_PRIORITY + 1, nullptr);
  xTaskCreate(rotary_sw_task, "rotary_encoder_sw", 256, (void *)&rt_sw,
              tskIDLE_PRIORITY + 1, nullptr);
  xTaskCreate(display_task, "SSD1306", 512, (void *)&dp0, tskIDLE_PRIORITY + 1,
              nullptr);
  vQueueAddToRegistry(dp0.comm_rot, "rotary_queue");
  vQueueAddToRegistry(dp0.comm_sw, "rotary_sw_queue");
  vQueueAddToRegistry(sensor_1_queue, "sensor_1_queue");

  vTaskStartScheduler();

  while (true) {
  };
}

#include "ssd1306os.h"
void display_task(void *param) {
  auto tpr = (display_params *)param;
  auto i2cbus{std::make_shared<PicoI2C>(1, 400000)};
  ssd1306os display(i2cbus);
  static bool next;
  // Screens
  auto tem = std::make_shared<InfoItem>("Hum & Tem", display);
  auto pres = std::make_shared<InfoItem>("Pressure", display);
  auto co = std::make_shared<InfoItem>("CO2", display);
  // Update Screens' data
  tem->updateValue("Temp", std::to_string(72) + "%");
  tem->updateValue("Humidity", std::to_string(60) + "%");
  pres->updateValue("Pres", std::to_string(80) + "%");
  co->updateValue("CO2", std::to_string(70) + "%");
  // Menu
  auto main_menu = std::make_shared<Menu>("Main menu", display);
  main_menu->add_item(tem);
  main_menu->add_item(pres);
  main_menu->add_item(co);

  while (true) {
    main_menu->show();
    //  INPUTS
    //      ROTTER INPUT
    while (xQueueReceive(tpr->comm_rot, &next, 0)) {
      if (next) {
        main_menu->event(MenuItem::up);
      } else {
        main_menu->event(MenuItem::down);
      }
    }
    //      ROTTER BUTTON INPUT
    if (xQueueReceive(tpr->comm_sw, &next, 0)) {
      main_menu->event(MenuItem::ok);
    }
    // EXAMPLES (how to update data using the incoming Queue data)
    if (false /*xQueueReceive(tpr->comm_temp, &temperature, 0)*/) {
      tem->updateValue("Temp", std::to_string(72 /*temperature*/) + "%");
    }
    if (false /*xQueueReceive(tpr->comm_hum, &humidity, 0)*/) {
      tem->updateValue("Humidity", std::to_string(60 /*humidity*/) + "%");
    }
    if (false /*xQueueReceive(tpr->comm_pres, &pressure, 0)*/) {
      pres->updateValue("Pres", std::to_string(80 /*pressure*/) + "%");
    }
    uint16_t co2_que_value;
    if (xQueueReceive(tpr->sensor_1_queue, &co2_que_value, 0)) {
      printf("%d", co2_que_value);
      co->updateValue("CO2", std::to_string(70 /*co2*/) + "%");
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
