#include "CO2Sensor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "nanomodbus.h"
#include <cstring>
#include <memory>
#include <iostream>

CO2Sensor::CO2Sensor(int uart_nr, int tx_pin, int rx_pin, int baudrate, uint8_t modbus_address)
    : address(modbus_address)
{
    uart = std::make_shared<PicoOsUart>(uart_nr, tx_pin, rx_pin, baudrate);
    modbus = std::make_shared<ModbusClient>(uart);
    modbus->set_destination_rtu_address(address);
}

bool CO2Sensor::readCO2(float &ppm)
{
    uint16_t regs[2];
    nmbs_error err = modbus->read_input_registers(0x0000, 2, regs);

    if (err != NMBS_ERROR_NONE)
    {
        std::cerr << "Modbus read error: " << err << std::endl;
        return false;
    }

    uint32_t raw = (regs[0] << 16) | regs[1];
    std::memcpy(&ppm, &raw, sizeof(float));
    return true;
}

void CO2Sensor::startTask(uint32_t interval_ms)
{
    xTaskCreate(taskFunc, "CO2Task", 1024, this, tskIDLE_PRIORITY + 1, nullptr);
}

void CO2Sensor::taskFunc(void *param)
{
    CO2Sensor *sensor = static_cast<CO2Sensor *>(param);
    float ppm;
    while (true)
    {
        if (sensor->readCO2(ppm))
        {
            std::cout << "CO₂ Concentration: " << ppm << " ppm" << std::endl;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
