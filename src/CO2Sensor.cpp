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

CO2Sensor::CO2Sensor(int uart_nr, int tx_pin, int rx_pin, int baudrate,
                     uint8_t modbus_address)
    : address(modbus_address),
      uart(std::make_shared<PicoOsUart>(uart_nr, tx_pin, rx_pin, baudrate, 2)),
      modbus(std::make_shared<ModbusClient>(uart)), ppm(modbus, 240, 0x0100) {}

uint16_t CO2Sensor::readCO2() {
  /*produal.write(100);
  vTaskDelay((100));
  produal.write(100);*/
  //printf("PPM=%5.1f%%\n", ppm.read() / 10.0);
  vTaskDelay(5);
  vTaskDelay(3000);
  printf("CO2 value from sensor class: %d\n",ppm.read());
  return ppm.read();
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
        uint16_t data = self->readCO2();
        xQueueSendToBack(q, &data, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
