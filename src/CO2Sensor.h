#pragma once

#include <memory>
#include <cstdint>
#include "PicoOsUart.h"
#include "ModbusClient.h"
#include "ModbusRegister.h"
struct sensor_co2_params {
    QueueHandle_t comm_co2;
};
class CO2Sensor {
public:
    // Constructor
    CO2Sensor(int uart_nr, int tx_pin, int rx_pin, int baudrate, uint8_t modbus_address, QueueHandle_t queue, std::shared_ptr<PicoOsUart> uart);

    // Read CO2 value from sensor (in ppm)
    bool readCO2();

    // Optional: start FreeRTOS task to poll periodically
    void startTask(uint32_t interval_ms);

private:
    std::shared_ptr<PicoOsUart> uart;
    std::shared_ptr<ModbusClient> modbus;
    uint8_t address;

    ModbusRegister ppm;
    QueueHandle_t queue;


    // FreeRTOS task function
    static void taskFunc(void* param);
};