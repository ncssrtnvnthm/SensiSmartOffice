#ifndef RECORD_H
#define RECORD_H

#include <WString.h>

class uiInterface;

enum class SensorValueType : uint8_t
{
    TEMPERATURE,
    HUMIDITY,
    PRESSURE
};

struct Record
{
    String DeviceName;
    Record(String deviceName);
    virtual ~Record();

    virtual String toString() = 0;

    virtual void accept(uiInterface *ui) = 0;
};

struct InfoRecord : Record
{
    String Info;
    InfoRecord(
        String deviceName,
        String info)
        : Record(deviceName), Info(info){};
    ~InfoRecord();

    String toString() { return DeviceName + " > " + Info; }

    void accept(uiInterface *ui);
};

struct ErrorRecord : Record
{
    String Error;
    ErrorRecord(
        String deviceName,
        String error)
        : Record(deviceName), Error(error){};
    ~ErrorRecord();

    void accept(uiInterface *ui);

    String toString() { return DeviceName + " > ERROR > " + Error; }
};

struct MeasureRecord : Record
{
    SensorValueType Type;
    float Value;

    MeasureRecord(
        String deviceName,
        SensorValueType type,
        float value)
        : Record(deviceName), Type(type), Value(value){};
    ~MeasureRecord();

    void accept(uiInterface *ui);

    String toString() { return DeviceName + " > " + mapName(Type) + " : " + Value + mapUnit(Type); }

    String mapName(SensorValueType st)
    {
        switch (st)
        {
        case SensorValueType::TEMPERATURE:
            return "Temperature";
        case SensorValueType::HUMIDITY:
            return "Humidity";
        case SensorValueType::PRESSURE:
            return "Pressure";
        default:
            return "Undefined";
        }
    }

    String mapUnit(SensorValueType st)
    {
        switch (st)
        {
        case SensorValueType::TEMPERATURE:
            return " °C";
        case SensorValueType::HUMIDITY:
            return " %";
        case SensorValueType::PRESSURE:
            return " hPa";
        default:
            return "";
        }
    }
};

#endif /* RECORD_H */
