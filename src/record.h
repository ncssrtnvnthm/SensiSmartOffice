#ifndef RECORD_H
#define RECORD_H

#include <WString.h>

class uiInterface;

enum class SensorValueType : uint8_t
{
    TEMPERATURE,
    HUMIDITY,
    PRESSURE,
    CO2,
    VOC_INDEX,
    NOX_INDEX,
    PM1P0,
    PM2P5,
    PM4P0,
    PM10P0,
    HCHO
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
        case SensorValueType::CO2:
            return "CO2";
        case SensorValueType::VOC_INDEX:
            return "VOC Index";
        case SensorValueType::NOX_INDEX:
            return "NOx Index";
        case SensorValueType::PM1P0:
            return "PM1.0";
        case SensorValueType::PM2P5:
            return "PM2.5";
        case SensorValueType::PM4P0:
            return "PM4.0";
        case SensorValueType::PM10P0:
            return "PM10";
        case SensorValueType::HCHO:
            return "Formaldehyde";
        default:
            return "Undefined";
        }
    }

    String mapUnit(SensorValueType st)
    {
        switch (st)
        {
        case SensorValueType::TEMPERATURE:
            return " &deg;C";
        case SensorValueType::HUMIDITY:
            return " %";
        case SensorValueType::PRESSURE:
            return " hPa";
        case SensorValueType::CO2:
            return " ppm";
        case SensorValueType::VOC_INDEX:
            return "";
        case SensorValueType::NOX_INDEX:
            return "";
        case SensorValueType::PM1P0:
            return " &micro;g/m³";
        case SensorValueType::PM2P5:
            return " &micro;g/m³";
        case SensorValueType::PM4P0:
            return " &micro;g/m³";
        case SensorValueType::PM10P0:
            return " &micro;g/m³";
        case SensorValueType::HCHO:
            return " ppb";
        default:
            return "";
        }
    }
};

#endif /* RECORD_H */
