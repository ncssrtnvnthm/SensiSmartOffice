#if defined(LCD) || defined(Compound)
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
        lcd.setCursor(0, _statusRow);
        lcd.print(buf);
    } else {
        lcd.setCursor(0, _statusRow);
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
        snprintf(buf, sizeof(buf), "T: %.1f C", record->Value);
        lcd.setCursor(0, 0);
        lcd.print(buf);
        for (int i = strlen(buf); i < LCD_COLUMNS; i++) lcd.print(' ');
        break;

    case SensorValueType::HUMIDITY:
        snprintf(buf, sizeof(buf), "H: %.1f %%", record->Value);
        lcd.setCursor(0, 1);
        lcd.print(buf);
        for (int i = strlen(buf); i < LCD_COLUMNS; i++) lcd.print(' ');
        break;

    case SensorValueType::PRESSURE:
        snprintf(buf, sizeof(buf), "P: %.1f hPa", record->Value);
        lcd.setCursor(0, 2);
        lcd.print(buf);
        for (int i = strlen(buf); i < LCD_COLUMNS; i++) lcd.print(' ');
        break;
    }
}

#endif /* LCD || Compound */
