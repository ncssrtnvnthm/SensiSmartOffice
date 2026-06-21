#include "sensor.h"
#include <cstdio>

void Sensor::HandleError(String prefix, uint16_t error)
{
    if (!error) return;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "0x%04X", error);
    _parent->push(new ErrorRecord(_name, prefix + buffer));
}
