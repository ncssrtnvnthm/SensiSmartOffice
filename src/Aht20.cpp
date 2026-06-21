#include "Aht20.h"

void Aht20Sensor::begin()
{
    if (!aht.begin())
    {
        _parent->push(new ErrorRecord(_name, "AHT20 init failed"));
        return;
    }
}

void Aht20Sensor::read()
{
    sensors_event_t humEvent, tempEvent;
    if (!aht.getEvent(&humEvent, &tempEvent))
    {
        _parent->push(new ErrorRecord(_name, "AHT20 read failed"));
        return;
    }

    temperature = tempEvent.temperature;
    humidity = humEvent.relative_humidity;

    pushRecords();
}

void Aht20Sensor::pushRecords()
{
    _parent->push(new MeasureRecord(_name, SensorValueType::TEMPERATURE, temperature));
    _parent->push(new MeasureRecord(_name, SensorValueType::HUMIDITY, humidity));
}
