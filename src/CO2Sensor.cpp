#include "CO2Sensor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "nanomodbus.h"
#include "ModbusRegister.h"
#include <cstring>
#include <memory>




CO2Sensor::CO2Sensor(int uart_nr, int tx_pin, int rx_pin, int baudrate, uint8_t modbus_address, QueueHandle_t queue, std::shared_ptr<PicoOsUart> uart)
        : address(modbus_address),
          uart(std::move(uart)),
          modbus(std::make_shared<ModbusClient>(uart)),
          ppm(modbus, 241, 0x0100),
          queue(queue)
{

}


bool CO2Sensor::readCO2()
{
    /*produal.write(100);
    vTaskDelay((100));
    produal.write(100);*/
    printf("PPM=%5.1f%%\n", ppm.read() / 10.0);
    vTaskDelay(5);
    vTaskDelay(3000);

    return true;
}
void CO2Sensor::startTask(uint32_t interval_ms)
{

    xTaskCreate(taskFunc, "CO2Task", 1024, (void*) queue, tskIDLE_PRIORITY + 1, nullptr);
}


void CO2Sensor::taskFunc(void *param)
{
    auto tpr = (QueueHandle_t)param;

    CO2Sensor *self = static_cast<CO2Sensor*>(param);
          while (1){
        auto data = self->readCO2();
        xQueueSendToBack(tpr, (void *)&data, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}




