# SensiSmartOffice

ESP32-based indoor climate monitor with AHT20 temperature/humidity sensor, BMP280 pressure sensor, 20×4 I2C LCD, BLE (Sensirion MyAmbience app), and captive-portal web dashboard.

## Hardware

| Component | Model / Address |
|---|---|
| MCU | ESP32 (uPesy Wroom) |
| Temperature / Humidity | AHT20 (I2C 0x38) |
| Pressure | BMP280 (I2C 0x76 / 0x77) |
| Display | 20×4 character LCD with PCF8574 I2C backpack (0x27) |

### Wiring

```
ESP32          AHT20     BMP280     LCD (PCF8574)
-----          -----     ------     -------------
GPIO 21 (SDA)  SDA       SDA        SDA
GPIO 22 (SCL)  SCL       SCL        SCL
3.3V           VCC       VCC        VCC
GND            GND       GND        GND
```

## Features

- **Sensor readings**: Temperature, humidity, and pressure every 1 second
- **20×4 LCD display**:
  ```
  TEMP: 23.5          C
  RH:   45.2          %
  P:    1013.2      hPa
        2026/06/23 14:30
  ```
  Labels are fixed-width — all numeric values start in the same column. Date/time is centered on the status row.

- **Captive portal**: Connect to WiFi `esp32_sensor_ap` — auto-opens dashboard. Handles probe URLs for iOS, Android, and Windows silently.
- **Web dashboard**: Dark theme, live value cards, scrollable log, auto-refresh every 5 seconds
- **BLE**: Sensirion MyAmbience app (temperature + humidity via `T_RH_V3` data type)
- **Time sync**: Two sources — NTP (via WiFi station, with automatic DST) or client browser timestamp on captive portal load
- **Automatic DST**: POSIX TZ string handles CET ↔ CEST transitions at EU dates (last Sunday of March/October)

## Build Environments

| Environment | Flag | UI Outputs |
|---|---|---|
| `upesy_wroom_WEB` | `-D WEB` | Captive portal web server |
| `upesy_wroom_LCD` | `-D LCD` | 20×4 LCD display |
| `upesy_wroom_BLE` | `-D USE_BLE` | BLE gadget (MyAmbience) |
| `upesy_wroom_compound` | `-D COMPOUND` | Web + LCD + BLE |

## Configuration

Edit `src/config.h` — all settings are in one file:

```cpp
// --- I²C Bus ---
#define I2C_SDA         21
#define I2C_SCL         22
#define I2C_CLOCK       100000UL      // 100 kHz standard

// --- Timing (ms) ---
#define MEASUREMENT_INTERVAL_MS  1000    // Sensor read interval
#define LOOP_DELAY_MS            3       // Loop yield
#define SENSOR_INIT_DELAY_MS     500     // Gap between sensor inits
#define COMMIT_DELAY_MS          200     // Post-commit pause

// --- WiFi Access Point (captive portal) ---
#define WIFI_AP_SSID        "esp32_sensor_ap"
#define WIFI_AP_PASSWORD    nullptr      // nullptr = open network

// --- WiFi Station (optional — for NTP time sync) ---
// Leave SSID empty to skip; time will sync from client browser instead.
#define WIFI_STA_SSID       ""
#define WIFI_STA_PASSWORD   ""

// --- NTP Time ---
#define NTP_SERVER          "pool.ntp.org"
// POSIX TZ string: "<STD>offset<DST>,start,end"
// CET-1  = UTC+1 (winter), CEST = UTC+2 (summer)
// M3.5.0 = DST starts last Sunday of March at 2:00
// M10.5.0/3 = DST ends last Sunday of October at 3:00
// Change the string for your timezone (see `man tzset` POSIX format).
#define POSIX_TZ_STRING     "CET-1CEST,M3.5.0,M10.5.0/3"
#define NTP_SYNC_INTERVAL_MS 3600000   // Re-sync hourly (NTP only)

// --- Web Server ---
#define WEB_SERVER_PORT         80
#define WEB_MAX_RECORDS         30
#define WEB_REFRESH_SECONDS     5

// --- LCD Display (PCF8574 I²C backpack) ---
#define LCD_I2C_ADDRESS  0x27
#define LCD_COLUMNS      20
#define LCD_ROWS         4
```

### Timezone examples

| Region | `POSIX_TZ_STRING` |
|---|---|
| Central Europe (CET/CEST) | `CET-1CEST,M3.5.0,M10.5.0/3` |
| UK (GMT/BST) | `GMT0BST,M3.5.0,M10.5.0/3` |
| US Eastern (EST/EDT) | `EST5EDT,M3.2.0,M11.1.0` |
| US Pacific (PST/PDT) | `PST8PDT,M3.2.0,M11.1.0` |
| Japan (JST, no DST) | `JST-9` |
| UTC | `UTC0` |

## Dependencies

| Library | Version | Used by |
|---|---|---|
| `adafruit/Adafruit AHTX0` | ^2.0.6 | All envs |
| `adafruit/Adafruit BMP280 Library` | ^3.0.0 | All envs |
| `robtillaart/I2C_LCD` | ^0.2.6 | All envs |
| `sensirion/Sensirion Gadget BLE Arduino Lib` | ^1.5.0 | BLE, Compound |
| `sensirion/Sensirion UPT Core` | 0.5.1 | BLE, Compound |
| `h2zero/NimBLE-Arduino` | ^2.3.0 | BLE, Compound |

## Build & Upload

```bash
# Build all environments
platformio run

# Upload specific environment
platformio run -e upesy_wroom_compound --target upload

# Monitor serial output
platformio device monitor
```

## Project Structure

```
src/
  main.cpp              — Entry point, loop, NTP sync
  config.h              — All configuration constants
  sensorContainer.*     — Manages sensor list, read/visit cycle
  sensor.*              — Base class for I2C sensors
  Aht20.*               — AHT20 temperature + humidity
  Bmp280.*              — BMP280 pressure (auto-detect 0x76/0x77)
  record.*              — Record types (Info, Error, Measure) + Visitor
  uiInterface.h         — Abstract UI backend (Visitor pattern)
  Web.*                 — Web server + captive portal dashboard
  LcdDisplay.*          — 20×4 I2C LCD display
  gadgetBle.*           — Sensirion BLE gadget (MyAmbience)
  compoundUi.*          — Composite UI (Web + LCD + BLE)
  wifi.cpp              — WiFi AP, station, NTP setup
  BLEProtocol.h         — Sensirion BLE data type definitions
  Measurement.h         — UPT measurement structures
  SignalType.h          — Signal type enum
  Sensirion_UPT_Core.h  — UPT core integration header
  Scd4x.h / Sen5x.h / Sfa3x.h — Stubs for future sensors (.cpp.unused)
```

## Architecture

The project uses the **Visitor pattern** for UI backends. Each sensor pushes `MeasureRecord` / `ErrorRecord` objects into a shared queue. The active UI backend visits each record and renders it appropriately — web cards, LCD lines, or BLE sample slots. This makes adding new sensors or UI backends straightforward without touching existing code.
