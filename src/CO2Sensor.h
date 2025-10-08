#pragma once

#include "ModbusClient.h"
#include "ModbusRegister.h"
#include "PicoOsUart.h"
#include <cstdint>
#include <memory>

class CO2Sensor {
public:
  // Constructor
  CO2Sensor(int uart_nr, int tx_pin, int rx_pin, int baudrate,
            uint8_t modbus_address);

  // Read CO2 value from sensor (in ppm)
  bool readCO2();

  // Optional: start FreeRTOS task to poll periodically
  void startTask(QueueHandle_t queue);

private:
  std::shared_ptr<PicoOsUart> uart;
  std::shared_ptr<ModbusClient> modbus;
  uint8_t address;

  ModbusRegister ppm;

  // FreeRTOS task function
  static void taskFunc(void *param);
};
