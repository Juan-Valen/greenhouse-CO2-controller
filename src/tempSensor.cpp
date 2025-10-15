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

Hmp60sensor::Hmp60sensor(const std::shared_ptr<ModbusClient> &rtu_client, const int slave=241)
        : rh_reg(rtu_client, slave, 256), t_reg(rtu_client, slave, 257) {}

        Sensor Hmp60sensor ::read() {
        vTaskDelay(5);
        vTaskDelay(3000);
        float rh_raw=rh_reg.read();
        float t_raw=t_reg.read();
        data.rh=rh_raw/10.0f;
        data.t=t_raw/10.0f;
        return data;
}
void Hmp60sensor::startTask(QueueHandle_t queue) {
    this->queue=queue;
    xTaskCreate(taskFunc,"tempSensor",1024,queue,1, nullptr);
}
void Hmp60sensor::taskFunc(void *param) {
    Hmp60sensor*self= static_cast<Hmp60sensor*> (param);
    while(1){
    auto data=self->read();
    xQueueSendToBack((QueueHandle_t)param,(void*)&data,0);
    vTaskDelay(pdMS_TO_TICKS(1000));
}
}
