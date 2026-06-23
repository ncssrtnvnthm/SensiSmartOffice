#ifndef SENSOR_CONTAINER_H
#define SENSOR_CONTAINER_H

#include "sensor.h"
#include <queue>

class Sensor;
class SensorContainer
{
private:
    typedef void (*SensorCallback)(Sensor* sensor);
    std::vector<Sensor*> _sensors;
    std::queue<Record*> _records;

    template<typename _Function>
    void forEachSensors(_Function func);

public:
    void begin();
    void read();
    void push(Record* record) {_records.push(record);}

    void accept(uiInterface* ui);
};

#endif /* SENSOR_CONTAINER_H */
