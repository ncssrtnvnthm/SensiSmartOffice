#if defined(LCD) || defined(Compound)
#include "config.h"
#include "LcdDisplay.h"

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

void LcdDisplay::commitMeasures() {}
void LcdDisplay::handleNetwork() {}

void LcdDisplay::visit(InfoRecord *record)
{
    lcd.setCursor(0, _statusRow);
    for (int i = 0; i < LCD_COLUMNS; i++) lcd.print(' ');
    lcd.setCursor(0, _statusRow);
    lcd.print(record->Info.substring(0, LCD_COLUMNS));
}

void LcdDisplay::visit(ErrorRecord *record)
{
    lcd.setCursor(0, _statusRow);
    for (int i = 0; i < LCD_COLUMNS; i++) lcd.print(' ');
    lcd.setCursor(0, _statusRow);
    lcd.print("ERR:");
    lcd.print(record->Error.substring(0, LCD_COLUMNS - 4));
}

void LcdDisplay::visit(MeasureRecord *record)
{
    char buf[21];
    switch (record->Type)
    {
    case SensorValueType::TEMPERATURE:
        snprintf(buf, sizeof(buf), "T:%.1f C", record->Value);
        lcd.setCursor(0, 0);
        lcd.print(buf);
        for (int i = strlen(buf); i < LCD_COLUMNS; i++) lcd.print(' ');
        break;

    case SensorValueType::HUMIDITY:
        snprintf(buf, sizeof(buf), "H:%.1f %%", record->Value);
        lcd.setCursor(0, 1);
        lcd.print(buf);
        for (int i = strlen(buf); i < LCD_COLUMNS; i++) lcd.print(' ');
        break;

    case SensorValueType::PRESSURE:
        snprintf(buf, sizeof(buf), "P:%.1f hPa", record->Value);
        lcd.setCursor(0, 2);
        lcd.print(buf);
        for (int i = strlen(buf); i < LCD_COLUMNS; i++) lcd.print(' ');
        break;
    }
}

#endif /* LCD || Compound */
