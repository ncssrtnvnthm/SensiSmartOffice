#ifndef UIINTERFACE_H
#define UIINTERFACE_H

struct InfoRecord;
struct ErrorRecord;
struct MeasureRecord;
class uiInterface
{
public:
    virtual void begin() = 0;

    virtual void visit(InfoRecord* record) = 0;
    virtual void visit(ErrorRecord* record) = 0;
    virtual void visit(MeasureRecord* record) = 0;

    virtual void commitMeasures() = 0;
    virtual void handleNetwork() = 0;
};

#endif /* UIINTERFACE_H */
