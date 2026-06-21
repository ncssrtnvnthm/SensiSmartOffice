#include <Arduino.h>
#include "config.h"
#include "sensorContainer.h"
#ifdef WEB
#include "Web.h"
#endif
#ifdef USE_BLE
#include "gadgetBle.h"
#endif
#ifdef Compound
#include "compoundUi.h"
#endif
#ifdef LCD
#include "LcdDisplay.h"
#endif

uiInterface *ui;
ulong iteration = 0;
SensorContainer sensors;
static int measurementIntervalMs = MEASUREMENT_INTERVAL_MS;
int64_t lastMeasurementTimeMs = 0;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println("\nI2C Scanner");
  pinMode(18, OUTPUT);

  sensors.begin();

#ifdef WEB
  ui = new Web();
#endif
#ifdef USE_BLE
  ui = new GadgetBle();
#endif
#ifdef Compound
  ui = new CompoundUi();
#endif
#ifdef LCD
  ui = new LcdDisplay();
#endif

  ui->begin();
}

void loop()
{
  ++iteration;

  if (millis() - lastMeasurementTimeMs >= measurementIntervalMs)
  {

    auto info = InfoRecord("Iteration", String(iteration));
    ui->visit(&info);

    sensors.read();

    sensors.accept(ui);

    ui->commitMeasures();
    delay(COMMIT_DELAY_MS);
    Serial.print(".");
    
    lastMeasurementTimeMs = millis();    
  };
  
  ui->handleNetwork();
  delay(LOOP_DELAY_MS);
}
