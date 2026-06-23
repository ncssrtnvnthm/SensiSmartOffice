#include <Arduino.h>
#include "config.h"
#include "sensorContainer.h"
#include <time.h>

// Forward-declare time helpers (defined in wifi.cpp for WEB/COMPOUND envs)
#if defined(WEB) || defined(COMPOUND)
bool IsWifiStationConnected();
bool IsTimeSynced();
#endif

#ifdef WEB
#include "Web.h"
#endif
#ifdef USE_BLE
#include "gadgetBle.h"
#endif
#ifdef COMPOUND
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
#if defined(WEB) || defined(COMPOUND)
  if (!IsWifiStationConnected()) return;
  if (millis() - lastNtpSyncMs < NTP_SYNC_INTERVAL_MS && lastNtpSyncMs != 0) return;

  // configTime() already called once in Setup_Wifi_Station();
  // sntp handles background re-sync automatically.
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
#elif defined(USE_BLE)
  ui = new GadgetBle();
#elif defined(COMPOUND)
  ui = new CompoundUi();
#elif defined(LCD)
  ui = new LcdDisplay();
#else
  #error "No UI backend defined (WEB, USE_BLE, COMPOUND, or LCD)"
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
