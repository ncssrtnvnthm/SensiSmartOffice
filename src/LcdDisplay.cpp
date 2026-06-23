#if defined(LCD) || defined(COMPOUND)
#include "config.h"
#include "LcdDisplay.h"
#include <time.h>

void LcdDisplay::clearRow(int row) {
    lcd.setCursor(0, row);
    for (int i = 0; i < LCD_COLUMNS; i++) lcd.print(' ');
}

void LcdDisplay::showTime() {
    struct tm ti;
    clearRow(_statusRow);
    if (getLocalTime(&ti, 0)) {
        char buf[21];
        strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M", &ti);
        lcd.setCursor(2, _statusRow);
        lcd.print(buf);
    } else {
        lcd.setCursor(2, _statusRow);
        lcd.print("----/--/-- --:--");
    }
}

void LcdDisplay::begin()
{
    lcd.begin(LCD_COLUMNS, LCD_ROWS);
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SensiSmartOffice");
    lcd.setCursor(0, 1);
    lcd.print("AHT20 + BMP280");
    lcd.setCursor(0, 2);
    lcd.print("I2C LCD 4x20");
    lcd.setCursor(0, 3);
    lcd.print("Starting...");
    delay(1500);
    lcd.clear();
}

void LcdDisplay::commitMeasures() {
    // Revert to clock after error timeout expires
    if (!_showingClock && millis() - _lastStatusMs > STATUS_TIMEOUT_MS) {
        _showingClock = true;
    }
    if (_showingClock) {
        showTime();
    }
}
void LcdDisplay::handleNetwork() {}

void LcdDisplay::visit(InfoRecord *record)
{
    // Info messages don't override the clock — only errors do
}

void LcdDisplay::visit(ErrorRecord *record)
{
    _showingClock = false;
    _lastStatusMs = millis();
    clearRow(_statusRow);
    lcd.setCursor(0, _statusRow);
    lcd.print("ERR:");
    lcd.print(record->Error.substring(0, LCD_COLUMNS - 4));
    _lastStatusMs = millis();
}

void LcdDisplay::visit(MeasureRecord *record)
{
    char buf[21];
    switch (record->Type)
    {
    case SensorValueType::TEMPERATURE:
        snprintf(buf, sizeof(buf), "%-5s %-12.1f %-1s", "TEMP:", record->Value, "C");
        lcd.setCursor(0, 0);
        lcd.print(buf);
        break;

    case SensorValueType::HUMIDITY:
        snprintf(buf, sizeof(buf), "%-5s %-12.1f %-1s", "RH:", record->Value, "%");
        lcd.setCursor(0, 1);
        lcd.print(buf);
        break;

    case SensorValueType::PRESSURE:
        snprintf(buf, sizeof(buf), "%-5s %-10.1f %-3s", "P:", record->Value, "hPa");
        lcd.setCursor(0, 2);
        lcd.print(buf);
        break;
    }
}

#endif /* LCD || COMPOUND */
