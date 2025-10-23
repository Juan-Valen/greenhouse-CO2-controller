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

Hmp60sensor::Hmp60sensor(const std::shared_ptr<ModbusClient> &rtu_client, int sensor, const int slave)
        : rh_reg(rtu_client, slave, 256), t_reg(rtu_client, slave, 257),
        sensor(sensor){}

float Hmp60sensor ::readTemperature() {
    float data;
    data = t_reg.read() / 10.0f;
    printf("Temperature from class %f\n", data);
    return data;
}
float Hmp60sensor::readHumidity() {
    float data;
    data = rh_reg.read() / 10.0f;
    printf("Humidity from class %f\n", data);
    return data;
}
void Hmp60sensor::startTask(QueueHandle_t queue) {
    auto *params = new hmp60_task_params{this, queue};
    xTaskCreate(taskFunc, "tempSensor", 1024, params, tskIDLE_PRIORITY + 1, nullptr);
}
void Hmp60sensor::taskFunc(void *param) {
    auto *p = static_cast<hmp60_task_params*>(param);
    Hmp60sensor *self = p->self;
    auto queue = p->queue;

    while (true) {
        auto temperature = self->readTemperature();
        auto humidity = self->readHumidity();
        if (self->sensor == 0){
            xQueueSendToBack(queue, &temperature, 0);
        }
        else if (self->sensor == 1){
            xQueueSendToBack(queue, &humidity, 0);
        }


        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}