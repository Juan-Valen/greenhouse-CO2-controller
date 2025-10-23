//
// Created by Eliezer Niyorukundo on 2025-10-14.
//

#ifndef HMP60_H
#define HMP60_H

#include <memory>
#include "modbus/ModbusRegister.h"
#include "modbus/ModbusClient.h"
#include "FreeRTOS.h"
#include "queue.h"

struct SensorData {
    float rh;
    float t;
};
class Hmp60sensor {
public:
    explicit Hmp60sensor(const std::shared_ptr<ModbusClient> &rtu_client, int sensor, const int slave=241);
    // sensor = 0 is temp sensor = 1 is humidity
    float readTemperature();
    float readHumidity();

    // Optional: start FreeRTOS task to poll periodically
    void startTask(QueueHandle_t queue);
private:
    ModbusRegister rh_reg;
    ModbusRegister t_reg;
    int sensor;

    static void taskFunc(void *param);
};

#endif // HMP60_H