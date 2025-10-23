//
// Created by Eliezer Niyorukundo on 2025-10-14.
//

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
#include "tempSensor.h"
struct hmp60_task_params {
    Hmp60sensor *self;
    QueueHandle_t queue;
};

Hmp60sensor::Hmp60sensor(const std::shared_ptr<ModbusClient> &rtu_client, const int slave)
        : rh_reg(rtu_client, slave, 256), t_reg(rtu_client, slave, 257) {}

        SensorData Hmp60sensor ::read() {
        vTaskDelay(5);
        vTaskDelay(3000);
        SensorData data;
        data.rh = rh_reg.read() / 10.0f;
        data.t = t_reg.read() / 10.0f;
        return data;
}
void Hmp60sensor::startTask(QueueHandle_t queue) {
    auto *params = new hmp60_task_params{this, queue};
    xTaskCreate(taskFunc, "tempSensor", 1024, params, 1, nullptr);
}
void Hmp60sensor::taskFunc(void *param) {
    auto *p = static_cast<hmp60_task_params*>(param);
    Hmp60sensor *self = p->self;
    auto q = p->queue;

    while (true) {
        auto data = self->read();
        xQueueSendToBack(q, &data, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
