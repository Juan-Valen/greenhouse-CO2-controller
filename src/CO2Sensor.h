#pragma once

#include "ModbusClient.h"
#include "ModbusRegister.h"
#include "PicoOsUart.h"
#include <cstdint>
#include <memory>

class CO2Sensor {
public:
  // Constructor
  explicit CO2Sensor(const std::shared_ptr<ModbusClient> &rtu_client, const int slave=240);

  // Read CO2 value from sensor (in ppm)
  double readCO2();

  // Optional: start FreeRTOS task to poll periodically
  void startTask(QueueHandle_t queue);

private:
  ModbusRegister ppm;

  // FreeRTOS task function
  static void taskFunc(void *param);
};
