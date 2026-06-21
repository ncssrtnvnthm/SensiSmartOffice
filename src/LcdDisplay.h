#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include "config.h"
#include "uiInterface.h"
#include "record.h"
#include <I2C_LCD.h>

class LcdDisplay : public uiInterface
{
private:
    I2C_LCD lcd;
    int _statusRow;

public:
    LcdDisplay()
        : lcd(LCD_I2C_ADDRESS), _statusRow(LCD_ROWS - 1) {}

    void begin();
    void commitMeasures();
    void handleNetwork();

    void visit(InfoRecord *record);
    void visit(ErrorRecord *record);
    void visit(MeasureRecord *record);
};

#endif /* LCDDISPLAY_H */
