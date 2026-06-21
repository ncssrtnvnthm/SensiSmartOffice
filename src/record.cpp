#include "uiInterface.h"
#include "record.h"

void InfoRecord::accept(uiInterface *ui) { ui->visit(this); }
void ErrorRecord::accept(uiInterface *ui) { ui->visit(this); }
void MeasureRecord::accept(uiInterface *ui) { ui->visit(this); }

Record::Record(String deviceName) : DeviceName(deviceName)
{
}

Record::~Record() {}

InfoRecord::~InfoRecord() {}
ErrorRecord::~ErrorRecord() {}
MeasureRecord::~MeasureRecord() {}
