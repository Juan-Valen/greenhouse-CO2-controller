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

CO2Sensor::CO2Sensor(int uart_nr, int tx_pin, int rx_pin, int baudrate,
                     uint8_t modbus_address)
    : address(modbus_address),
      uart(std::make_shared<PicoOsUart>(uart_nr, tx_pin, rx_pin, baudrate, 2)),
      modbus(std::make_shared<ModbusClient>(uart)), ppm(modbus, 241, 0x0100) {}

bool CO2Sensor::readCO2() {
  /*produal.write(100);
  vTaskDelay((100));
  produal.write(100);*/
  //printf("PPM=%5.1f%%\n", ppm.read() / 10.0);
  vTaskDelay(5);
  vTaskDelay(3000);

  return ppm.read();
}
void CO2Sensor::startTask(QueueHandle_t queue) {
  xTaskCreate(taskFunc, "CO2Task", 1024, queue, tskIDLE_PRIORITY + 1, nullptr);
}

void CO2Sensor::taskFunc(void *param) {
  CO2Sensor *self = static_cast<CO2Sensor *>(param);
  while (1) {
    auto data = self->readCO2();
    xQueueSendToBack((QueueHandle_t)param, (void *)&data, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
