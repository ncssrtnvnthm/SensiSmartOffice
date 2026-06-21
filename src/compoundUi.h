#ifndef COMPAUNDUI_H
#define COMPAUNDUI_H

#include "uiInterface.h"

class CompaundUi : public uiInterface
{
private:
    std::vector<uiInterface*> _children;

public:
    void begin();
    void commitMeasures();
    void handleNetwork();

    void visit(InfoRecord* record);
    void visit(ErrorRecord* record);
    void visit(MeasureRecord* record);
};

#endif /*COMPAUNDUI_H*/