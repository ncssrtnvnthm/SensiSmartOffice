#ifndef SENSOR_H
#define SENSOR_H

#include "record.h"
#include "sensorContainer.h"
#include <vector>

class SensorContainer;
class Sensor
{
protected:
    Sensor(const char *name, SensorContainer* parent)
    {
        _name = name;
        _parent = parent;
    }
    String _name;
    SensorContainer* _parent;

    void HandleError(String prefix, uint16_t error);


public:
    virtual void begin() = 0;
    virtual void read() = 0;
};

#endif /* SENSOR_H */
