#if defined(COMPOUND)
#include "compoundUi.h"

#include "Web.h"
#include "gadgetBle.h"
#include "LcdDisplay.h"

CompoundUi::~CompoundUi() {
    for (auto* child : _children) delete child;
    _children.clear();
}

void CompoundUi::begin(){
    _children.push_back(new Web());
    _children.push_back(new GadgetBle());
    _children.push_back(new LcdDisplay());
    std::for_each(_children.begin(), _children.end(), [](uiInterface* ui){ui->begin();});
}

void CompoundUi::commitMeasures(){
    std::for_each(_children.begin(), _children.end(), [](uiInterface* ui){ui->commitMeasures();});
}

void CompoundUi::handleNetwork(){
    std::for_each(_children.begin(), _children.end(), [](uiInterface* ui){ui->handleNetwork();});
}

void CompoundUi::visit(InfoRecord* record){
    std::for_each(_children.begin(), _children.end(), [record](uiInterface* ui){ui->visit(record);});
}

void CompoundUi::visit(ErrorRecord* record){
    std::for_each(_children.begin(), _children.end(), [record](uiInterface* ui){ui->visit(record);});
}

void CompoundUi::visit(MeasureRecord* record){
    std::for_each(_children.begin(), _children.end(), [record](uiInterface* ui){ui->visit(record);});
}

#endif /* COMPOUND */
