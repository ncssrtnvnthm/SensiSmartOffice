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
#define MEASUREMENT_INTERVAL_MS     5000
#define LOOP_DELAY_MS               3
#define SENSOR_INIT_DELAY_MS        500
#define COMMIT_DELAY_MS             500

// --- WiFi Access Point ---
#define WIFI_AP_SSID        "esp32_sensor_ap"
#define WIFI_AP_PASSWORD    NULL

// --- Web Server ---
#define WEB_SERVER_PORT         80
#define WEB_MAX_RECORDS         30
#define WEB_REFRESH_SECONDS     5

// --- LCD Display (PCF8574 I²C backpack) ---
#define LCD_I2C_ADDRESS  0x27
#define LCD_COLUMNS      20
#define LCD_ROWS         4

#endif /* CONFIG_H */
