#include "compaundUi.h"

#include "Web.h"
#include "gatgetBle.h"

void CompaundUi::begin(){
    _children.push_back(new Web());
    _children.push_back(new GatgetBle());
    std::for_each(_children.begin(), _children.end(), [](uiInterface* ui){ui->begin();});
}

void CompaundUi::commitMeasures(){
    std::for_each(_children.begin(), _children.end(), [](uiInterface* ui){ui->commitMeasures();});
}

void CompaundUi::handleNetwork(){
    std::for_each(_children.begin(), _children.end(), [](uiInterface* ui){ui->handleNetwork();});
}

void CompaundUi::visit(InfoRecord* record){
    std::for_each(_children.begin(), _children.end(), [record](uiInterface* ui){ui->visit(record);});
}

void CompaundUi::visit(ErrorRecord* record){
    std::for_each(_children.begin(), _children.end(), [record](uiInterface* ui){ui->visit(record);});
}

void CompaundUi::visit(MeasureRecord* record){
    std::for_each(_children.begin(), _children.end(), [record](uiInterface* ui){ui->visit(record);});
}
