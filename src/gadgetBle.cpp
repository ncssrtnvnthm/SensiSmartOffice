#if defined(USE_BLE) || defined(COMPOUND)
#include "gadgetBle.h"
#include "record.h"
#include "Sensirion_UPT_Core.h"
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;

DataProvider *provider;

void GadgetBle::begin()
{
  provider = new DataProvider(lib, DataType::T_RH_V3, true, false, &wifi);
  provider->begin();
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
    // T_RH_V3 expects: temperature first, then humidity
    // writeValueToCurrentSample encodes and places into the correct slot
    switch (record->Type)
    {
    case SensorValueType::TEMPERATURE:
        provider->writeValueToCurrentSample(record->Value,
            SignalType::TEMPERATURE_DEGREES_CELSIUS);
        break;
    case SensorValueType::HUMIDITY:
        provider->writeValueToCurrentSample(record->Value,
            SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
        break;
    default:
        return;
    }
}

#endif /* USE_BLE || COMPOUND */
