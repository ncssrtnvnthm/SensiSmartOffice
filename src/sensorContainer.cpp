#include "config.h"
#include "sensorContainer.h"
#include "Aht20.h"
#include "Bmp280.h"
#include <algorithm>
#include <Wire.h>
#include <Arduino.h>

void SensorContainer::begin()
{
    Wire.begin(I2C_SDA, I2C_SCL, I2C_CLOCK);

    _sensors.push_back(new Aht20Sensor("AHT20", this));
    _sensors.push_back(new Bmp280Sensor("BMP280", this));

    forEachSensors([](Sensor *sensor)
                   {sensor->begin();
                   delay(SENSOR_INIT_DELAY_MS); });
}

void SensorContainer::read()
{
    forEachSensors([](Sensor *sensor)
                   { sensor->read(); });
}

void SensorContainer::accept(uiInterface *ui)
{
    while (!_records.empty())
    {
        Record *record = _records.front();
        Serial.println(record->toString());
        record->accept(ui);
        _records.pop();
        delete record;
    }
}

template <typename _Function>
void SensorContainer::forEachSensors(_Function func)
{
    std::for_each(_sensors.begin(), _sensors.end(), func);
}
