#pragma once

#include <SimpleFOC.h>
#include "driver/spi_master.h"

class MT6701Sensor : public Sensor
{
public:
    MT6701Sensor();

    void init();

    float getSensorAngle();

private:
    spi_device_handle_t spi_device_;
    spi_transaction_t spi_transaction_ = {};

    float x_;
    float y_;
    uint32_t last_update_;
};
