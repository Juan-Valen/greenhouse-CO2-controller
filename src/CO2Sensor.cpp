#include "CO2Sensor.h"
#include "FreeRTOS.h"
#include "ModbusRegister.h"
#include "PicoI2C.h"
#include "PicoOsUart.h"
#include "nanomodbus.h"
#include "task.h"
#include <cstdint>
#include <cstring>
#include <memory>
#include <pico/types.h>
struct co2_task_params {
    CO2Sensor *self;
    QueueHandle_t queue;
};

CO2Sensor::CO2Sensor(const std::shared_ptr<ModbusClient> &rtu_client, const int slave)
: ppm(rtu_client, slave, 0x100){}

double CO2Sensor::readCO2() {
  auto c = ppm.read();
  printf("CO2 value from sensor class: %d\n",ppm.read());
  return c;
}
void CO2Sensor::startTask(QueueHandle_t queue) {
    auto *params = new co2_task_params{this, queue};
    xTaskCreate(taskFunc, "CO2Task", 1024, params, tskIDLE_PRIORITY + 1, nullptr);
}

void CO2Sensor::taskFunc(void *param) {
    auto *p = static_cast<co2_task_params *>(param);
    auto *self = p->self;
    auto q = p->queue;

    while (1) {
        auto data = self->readCO2();
        xQueueSendToBack(q, &data, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
