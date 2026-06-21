#ifndef BMP280_H
#define BMP280_H

#include "sensor.h"
#include <Adafruit_BMP280.h>

class Bmp280Sensor : public Sensor
{
private:
    static const uint8_t I2C_ADDR_1 = 0x76;
    static const uint8_t I2C_ADDR_2 = 0x77;
    Adafruit_BMP280 bmp;
    float pressure = 0.0f;
    uint8_t _foundAddress;
    bool tryBegin(uint8_t addr);
    void pushRecords();

public:
    Bmp280Sensor(const char *name, SensorContainer *parent) : Sensor(name, parent){};

    void begin();
    void read();
};

#endif /* BMP280_H */
