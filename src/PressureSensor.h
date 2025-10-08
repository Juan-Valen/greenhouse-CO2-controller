#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SDP610_ADDR          0x40
#define SDP610_SCALE_FACTOR  240.0f   // counts per Pascal (from datasheet)

class PressureSensor {
public:
    PressureSensor(i2c_inst_t *i2cPort = i2c1, uint8_t addr = SDP610_ADDR);

    // Initialize sensor
    void begin(uint sda_pin = 14, uint scl_pin = 15, uint baudrate = 100000);

    // Get raw sensor value
    int16_t readRaw();

    // Get converted pressure (Pa)
    float readPressurePa();

private:
    i2c_inst_t *_i2c;
    uint8_t _addr;
};

#endif