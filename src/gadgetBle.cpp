#if defined(USE_BLE) || defined(Compound)
#include "gadgetBle.h"
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;

DataProvider *provider;

void GadgetBle::begin()
{
  sampleConfigSelector[DataType::T_RH_V3].sampleType = (uint8_t)-1;
  provider = new DataProvider(lib, DataType::T_RH_V3, true, false, &wifi);
  provider->begin();
  provider->setSampleConfig(DataType::T_RH_CO2_VOC_PM25_HCHO);
  Serial.print("Sensirion GadgetBle Lib initialized with deviceId = ");
  Serial.println(provider->getDeviceIdString());
}

void GadgetBle::commitMeasures()
{
  provider->commitSample();
}

void GadgetBle::handleNetwork()
{
  provider->handleDownload();
}

void GadgetBle::visit(InfoRecord *record) {}
void GadgetBle::visit(ErrorRecord *record) {}
void GadgetBle::visit(MeasureRecord *record)
{
    SignalType st;
    switch (record->Type)
    {
    case SensorValueType::TEMPERATURE:
        st = SignalType::TEMPERATURE_DEGREES_CELSIUS;
        break;
    case SensorValueType::HUMIDITY:
        st = SignalType::RELATIVE_HUMIDITY_PERCENTAGE;
        break;
    default:
        return; // PRESSURE not supported by Sensirion BLE profile
    }
    provider->writeValueToCurrentSample(record->Value, st);
}

#endif /* USE_BLE || Compound */
