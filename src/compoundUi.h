#ifndef COMPOUND_UI_H
#define COMPOUND_UI_H

#include "uiInterface.h"
#include <vector>

class CompoundUi : public uiInterface
{
private:
    std::vector<uiInterface*> _children;

public:
    ~CompoundUi();

    void begin();
    void commitMeasures();
    void handleNetwork();

    void visit(InfoRecord* record);
    void visit(ErrorRecord* record);
    void visit(MeasureRecord* record);
};

#endif /* COMPOUND_UI_H */