#ifndef CONFIG_H
#define CONFIG_H

// ==========================================
// SensiSmartOffice — Central Configuration
// ==========================================

// --- I²C Bus ---
#define I2C_SDA         21
#define I2C_SCL         22
#define I2C_CLOCK       10000UL

// --- Serial ---
#define SERIAL_BAUD     115200

// --- Timing (milliseconds) ---
#define MEASUREMENT_INTERVAL_MS     1000    // How often sensors are read
#define LOOP_DELAY_MS               3       // Yield between loop iterations
#define SENSOR_INIT_DELAY_MS        500     // Pause between sensor inits at boot
#define COMMIT_DELAY_MS             200     // Pause after committing measurements

// --- WiFi Access Point ---
#define WIFI_AP_SSID        "esp32_sensor_ap"
#define WIFI_AP_PASSWORD    NULL

// --- WiFi Station (for NTP time sync) ---
// Set to your home WiFi to get internet time; leave SSID empty to skip
#define WIFI_STA_SSID       ""
#define WIFI_STA_PASSWORD   ""

// --- NTP Time ---
#define NTP_SERVER          "pool.ntp.org"
#define NTP_GMT_OFFSET_SEC  3600        // GMT+1 (CET); change for your timezone
#define NTP_DST_OFFSET_SEC  3600        // Summer time offset
#define NTP_SYNC_INTERVAL_MS 3600000     // Re-sync every hour

// --- Web Server ---
#define WEB_SERVER_PORT         80
#define WEB_MAX_RECORDS         30
#define WEB_REFRESH_SECONDS     5

// --- LCD Display (PCF8574 I²C backpack) ---
#define LCD_I2C_ADDRESS  0x27
#define LCD_COLUMNS      20
#define LCD_ROWS         4

#endif /* CONFIG_H */
