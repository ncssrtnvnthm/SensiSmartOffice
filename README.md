# SensiSmartOffice

ESP32-based indoor climate monitor with AHT20 temperature/humidity sensor, BMP280 pressure sensor, 20×4 I2C LCD, BLE (Sensirion MyAmbience app), and captive-portal web dashboard.

## Hardware

| Component | Model |
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
- **20×4 LCD display**: T, H, P on rows 0–2, date/time on row 3
- **Captive portal**: Connect to WiFi `esp32_sensor_ap` — auto-opens dashboard
- **Web dashboard**: Dark theme, live values, scrollable log
- **BLE**: Sensirion MyAmbience app (temperature + humidity)
- **Time sync**: Via NTP (WiFi station) or client browser on captive portal load
- **SVG charts**: Removed for simplicity (available in git history)

## Build Environments

| Environment | Flags | Outputs |
|---|---|---|
| `upesy_wroom_WEB` | `-D WEB` | Captive portal web server |
| `upesy_wroom_LCD` | `-D LCD` | 20×4 LCD display only |
| `upesy_wroom_BLE` | `-D USE_BLE` | BLE gadget (MyAmbience) |
| `upesy_wroom_compound` | `-D Compound` | Web + LCD + BLE |

## Configuration

Edit `src/config.h`:

```cpp
// WiFi AP for captive portal
#define WIFI_AP_SSID        "esp32_sensor_ap"

// WiFi Station for NTP time (optional)
#define WIFI_STA_SSID       "MyWiFi"
#define WIFI_STA_PASSWORD   "MyPassword"

// Timezone
#define NTP_GMT_OFFSET_SEC  3600   // GMT+1
#define NTP_DST_OFFSET_SEC  3600   // Summer time

// LCD
#define LCD_I2C_ADDRESS     0x27
#define LCD_COLUMNS         20
#define LCD_ROWS            4

// I2C pins
#define I2C_SDA             21
#define I2C_SCL             22
```

## Build & Upload

```bash
# Build all
platformio run

# Upload specific environment
platformio run -e upesy_wroom_compound --target upload
```
