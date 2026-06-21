#include <Arduino.h>
#include "config.h"
#include "sensorContainer.h"
#include <time.h>

// Forward-declare time helpers (defined in wifi.cpp for WEB/Compound envs)
#if defined(WEB) || defined(Compound)
bool IsWifiStationConnected();
bool IsTimeSynced();
#endif

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
int64_t lastNtpSyncMs = 0;

static void syncNtpTime() {
#if defined(WEB) || defined(Compound)
  if (!IsWifiStationConnected()) return;
  if (millis() - lastNtpSyncMs < NTP_SYNC_INTERVAL_MS && lastNtpSyncMs != 0) return;

  configTime(NTP_GMT_OFFSET_SEC, NTP_DST_OFFSET_SEC, NTP_SERVER);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 5000)) {
    lastNtpSyncMs = millis();
    Serial.print("NTP time: ");
    Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
  }
#endif
}

static String getTimeStr() {
  struct tm ti;
  if (getLocalTime(&ti, 0)) {
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", &ti);
    return String(buf);
  }
  return "--:--:--";
}

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

  // Initial NTP sync (non-blocking, runs after WiFi if available)
  syncNtpTime();
}

void loop()
{
  ++iteration;

  // Periodically sync NTP
  syncNtpTime();

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
