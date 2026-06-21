#ifndef GADGETBLE_H
#define GADGETBLE_H
#include "uiInterface.h"

class GadgetBle : public uiInterface
{
public:
    void begin();
    void commitMeasures();
    void handleNetwork();

    void visit(InfoRecord* record);
    void visit(ErrorRecord* record);
    void visit(MeasureRecord* record);
};

#endif /* GADGETBLE_H */
