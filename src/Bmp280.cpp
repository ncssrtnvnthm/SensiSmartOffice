#include "Bmp280.h"
#include <Arduino.h>

bool Bmp280Sensor::tryBegin(uint8_t addr)
{
    return bmp.begin(addr);
}

void Bmp280Sensor::begin()
{
    delay(10); // Power-up stabilization

    if (tryBegin(I2C_ADDR_1))
    {
        _foundAddress = I2C_ADDR_1;
        return;
    }
    if (tryBegin(I2C_ADDR_2))
    {
        _foundAddress = I2C_ADDR_2;
        return;
    }

    _parent->push(new ErrorRecord(_name, "BMP280 not found at 0x76 or 0x77"));
}

void Bmp280Sensor::read()
{
    float pressurePa = bmp.readPressure();
    if (pressurePa <= 0.0f)
    {
        _parent->push(new ErrorRecord(_name, "BMP280 read failed"));
        return;
    }

    pressure = pressurePa / 100.0f; // Pa → hPa

    pushRecords();
}

void Bmp280Sensor::pushRecords()
{
    _parent->push(new MeasureRecord(_name, SensorValueType::PRESSURE, pressure));
}
