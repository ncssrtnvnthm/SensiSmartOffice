#ifndef AHT20_H
#define AHT20_H

#include "sensor.h"
#include <Adafruit_AHTX0.h>

class Aht20Sensor : public Sensor
{
private:
    Adafruit_AHTX0 aht;
    float temperature = 0.0f;
    float humidity = 0.0f;
    void pushRecords();

public:
    Aht20Sensor(const char *name, SensorContainer *parent) : Sensor(name, parent){};

    void begin();
    void read();
};

#endif /* AHT20_H */
