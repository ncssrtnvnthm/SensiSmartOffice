#ifndef CompoundUI_H
#define CompoundUI_H

#include "uiInterface.h"
#include <vector>

class CompoundUi : public uiInterface
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

#endif /*CompoundUI_H*/