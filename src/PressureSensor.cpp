#include "PressureSensor.h"
#include "hardware/i2c.h"
#include <stdio.h>

// Constructor
PressureSensor::PressureSensor(i2c_inst_t *i2cPort, uint8_t addr)
    : _i2c(i2cPort), _addr(addr) {}

// Initialize I2C
void PressureSensor::begin(uint sda_pin, uint scl_pin, uint baudrate) {
    i2c_init(_i2c, baudrate);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
}

// Read raw value
int16_t PressureSensor::readRaw() {
    uint8_t buffer[2];
    int read = i2c_read_blocking(_i2c, _addr, buffer, 2, false);
    if (read == 2) {
        return (int16_t)((buffer[0] << 8) | buffer[1]);
    } else {
        printf("[SDP610] I2C read failed!\n");
        return 0;
    }
}

// Convert to Pascals
float PressureSensor::readPressurePa() {
    int16_t raw = readRaw();
    return static_cast<float>(raw) / SDP610_SCALE_FACTOR;
}