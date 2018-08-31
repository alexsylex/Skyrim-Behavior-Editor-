#include "genericdatawidgets.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

#include <QApplication>

#define BINDING_ITEM_LABEL QString("Use Property     ")

GenericTableWidget::GenericTableWidget(const QString & title)
    : lyt(new QGridLayout),
      table(new QTableWidget),
      resetFilterPB(new QPushButton("Reset Filter")),
      filterPB(new QPushButton("Filter By String")),
      filterLE(new QLineEdit),
      selectPB(new QPushButton("Select")),
      cancelPB(new QPushButton("Cancel")),
      //newPB(new QPushButton("New")),
      //typeSelector(new ComboBox),
      lastSelectedRow(-1)
{
    setWindowTitle(title);
    resize(500, 800);
    setWindowModality(Qt::ApplicationModal);
    QStringList list = {"Name", "Type"};
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(list);
    table->hideColumn(1);
    table->setShowGrid(false);
    //table->setMouseTracking(true);
    //table->setStyleSheet("QTableWidget { background:cyan }");
    table->setStyleSheet("QHeaderView::section { background-color:grey }");
    table->verticalHeader()->setVisible(true);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionsClickable(false);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lyt->addWidget(filterLE, 0, 0, 1, 3);
    lyt->addWidget(resetFilterPB, 0, 3, 1, 2);
    lyt->addWidget(filterPB, 0, 6, 1, 3);
    lyt->addWidget(table, 1, 0, 9, 10);
    lyt->addWidget(selectPB, 10, 2, 1, 2);
    lyt->addWidget(cancelPB, 10, 7, 1, 2);
    //lyt->addWidget(newPB, 9, 2, 1, 2);
    //lyt->addWidget(typeSelector, 9, 5, 1, 4);
    setLayout(lyt);
    connect(selectPB, SIGNAL(released()), this, SLOT(itemSelected()), Qt::UniqueConnection);
    //connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(itemSelected()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(itemSelectedAt(int,int)), Qt::UniqueConnection);
    connect(cancelPB, SIGNAL(released()), this, SLOT(hide()), Qt::UniqueConnection);
    //connect(newPB, SIGNAL(released()), this, SLOT(itemAdded()), Qt::UniqueConnection);
    connect(filterPB, SIGNAL(released()), this, SLOT(filterItems()), Qt::UniqueConnection);
    connect(resetFilterPB, SIGNAL(released()), this, SLOT(resetFilter()), Qt::UniqueConnection);
}

void GenericTableWidget::loadTable(const QStringList & names, const QStringList & types, const QString & firstElement){
    int start = 0;
    table->setRowCount(0);
    if (!types.isEmpty()){
        table->setColumnHidden(1, false);
    }
    if (names.size() == types.size()){
        if (firstElement != ""){
            start = 1;
            if (table->rowCount() > 0){
                if (table->item(0, 0)){
                    table->item(0, 0)->setText(firstElement);
                }else{
                    table->setItem(0, 0, new QTableWidgetItem(firstElement));
                }
                if (table->item(0, 1)){
                    table->item(0, 1)->setText(firstElement);
                }else{
                    table->setItem(0, 1, new QTableWidgetItem(firstElement));
                }
            }else{
                table->setRowCount(table->rowCount() + 1);
                table->setItem(0, 0, new QTableWidgetItem(firstElement));
                table->setItem(0, 1, new QTableWidgetItem(firstElement));
            }
        }
        for (auto i = start, j = 0; j < names.size(); i++, j++){
            if (i < table->rowCount()){
                if (table->item(i, 0)){
                    table->item(i, 0)->setText(names.at(j));
                }else{
                    table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
                }
                if (table->item(i, 1)){
                    table->item(i, 1)->setText(types.at(j));
                }else{
                    table->setItem(i, 1, new QTableWidgetItem(types.at(j)));
                }
            }else{
                table->setRowCount(table->rowCount() + 1);
                table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
                table->setItem(i, 1, new QTableWidgetItem(types.at(j)));
            }
        }
    }else{
        WARNING_MESSAGE(QString("GenericTableWidget: loadTable()\nThe stringlist arguments have different sizes!!!"));
    }
}

void GenericTableWidget::loadTable(const QStringList & names, const QString & type, const QString & firstElement){
    int start = 0;
    table->setRowCount(0);
    if (type != ""){
        table->setColumnHidden(1, false);
    }
    if (firstElement != ""){
        start = 1;
        if (table->rowCount() > 0){
            if (table->item(0, 0)){
                table->item(0, 0)->setText(firstElement);
            }else{
                table->setItem(0, 0, new QTableWidgetItem(firstElement));
            }
            if (table->item(0, 1)){
                table->item(0, 1)->setText(firstElement);
            }else{
                table->setItem(0, 1, new QTableWidgetItem(firstElement));
            }
        }else{
            table->setRowCount(table->rowCount() + 1);
            table->setItem(0, 0, new QTableWidgetItem(firstElement));
            table->setItem(0, 1, new QTableWidgetItem(firstElement));
        }
    }
    for (auto i = start, j = 0; j < names.size(); i++, j++){
        if (i < table->rowCount()){
            if (table->item(i, 0)){
                table->item(i, 0)->setText(names.at(j));
            }else{
                table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
            }
            if (table->item(i, 1)){
                table->item(i, 1)->setText(type);
            }else{
                table->setItem(i, 1, new QTableWidgetItem(type));
            }
        }else{
            table->setRowCount(table->rowCount() + 1);
            table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
            table->setItem(i, 1, new QTableWidgetItem(type));
        }
    }
}

int GenericTableWidget::getNumRows() const{
    return table->rowCount();
}

void GenericTableWidget::addItem(const QString & name, const QString & type){
    table->blockSignals(true);
    int i = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    table->setItem(i, 0, new QTableWidgetItem(name));
    table->setItem(i, 1, new QTableWidgetItem(type));
    table->setCurrentCell(table->rowCount() - 1, 0);
    table->blockSignals(false);
}

void GenericTableWidget::renameItem(int index, const QString & newname){
    table->blockSignals(true);
    if (index < table->rowCount() && index >= 0){
        table->item(index, 0)->setText(newname);
    }else{
        WARNING_MESSAGE("GenericTableWidget::renameItem(): \nInvalid index!!!");
    }
    table->blockSignals(false);
}

void GenericTableWidget::removeItem(int index){
    if (index < table->rowCount() && index >= 0){
        if (table->currentRow() == index){
            lastSelectedRow = -1;
        }
        table->removeRow(index);
    }
}

/*void GenericTableWidget::itemAdded(){
    addItem("New"+typeSelector->currentText(), typeSelector->currentText());
    emit elementAdded(table->rowCount() - 1, typeSelector->currentText());
}*/

void GenericTableWidget::showTable(int index, const QString & typeallowed, const QStringList &typesdisallowed){
    lastSelectedRow = index;
    /*for (auto i = 1; i < table->rowCount(); i++){
        if (table->isRowHidden(i)){
            table->setRowHidden(i, false);
        }
    }*/
    if (index < table->rowCount() && index >= 0){
        table->setCurrentCell(index, 0);
    }
    setTypeFilter(typeallowed, typesdisallowed);
    show();
}

void GenericTableWidget::showTable(const QString & name, const QString & typeallowed, const QStringList &typesdisallowed){
    int index = -1;
    for (auto i = 0; i < table->rowCount(); i++){
        /*if (table->isRowHidden(i)){
            table->setRowHidden(i, false);
        }*/
        if (table->item(i, 0) && table->item(i, 0)->text() == name){
            lastSelectedRow = i;
            break;
        }
    }
    index = lastSelectedRow;
    if (index < table->rowCount() && index >= 0){
        table->setCurrentCell(index, 0);
    }
    setTypeFilter(typeallowed, typesdisallowed);
    show();
}

void GenericTableWidget::filterItems(){
    QTableWidgetItem *item = nullptr;
    if (filterLE->text() != ""){
        for (auto i = 1; i < table->rowCount(); i++){
            item = table->item(i, 0);
            if (item){
                if (!item->text().contains(filterLE->text(), Qt::CaseInsensitive)){
                    table->hideRow(i);
                }
            }else{
                LogFile::writeToLog(QString("GenericTableWidget::filterItems(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
            }
        }
    }else{
        if (onlyTypeAllowed != ""){
            for (auto i = 1; i < table->rowCount(); i++){
                if (table->isRowHidden(i)){
                    item = table->item(i, 1);
                    if (item){
                        if (item->text() == onlyTypeAllowed){
                            table->setRowHidden(i, false);
                        }
                    }else{
                        LogFile::writeToLog(QString("GenericTableWidget::filterItems(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
                    }
                }
            }
        }else if (!typesDisallowed.isEmpty()){
            for (auto i = 1; i < table->rowCount(); i++){
                if (table->isRowHidden(i)){
                    item = table->item(i, 1);
                    if (item){
                        if (!typesDisallowed.contains(item->text())){
                            table->setRowHidden(i, false);
                        }
                    }else{
                        LogFile::writeToLog(QString("GenericTableWidget::filterItems(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
                    }
                }
            }
        }else{
            for (auto i = 1; i < table->rowCount(); i++){
                if (table->isRowHidden(i)){
                    table->setRowHidden(i, false);
                }
            }
        }
    }
}

void GenericTableWidget::resetFilter(){
    QTableWidgetItem *item = nullptr;
    if (onlyTypeAllowed != ""){
        if (table->columnCount() == 2 && !table->isColumnHidden(1)){
            for (auto i = 1; i < table->rowCount(); i++){
                item = table->item(i, 1);
                if (item){
                    if (item->text() != onlyTypeAllowed){
                        table->hideRow(i);
                    }else{
                        table->setRowHidden(i, false);
                    }
                }else{
                    LogFile::writeToLog(QString("GenericTableWidget::setTypeFilter(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
                }
            }
        }
    }else if (!typesDisallowed.isEmpty()){
        if (table->columnCount() == 2 && !table->isColumnHidden(1)){
            for (auto i = 1; i < table->rowCount(); i++){
                item = table->item(i, 1);
                if (item){
                    if (typesDisallowed.contains(item->text())){
                        table->hideRow(i);
                    }else{
                        table->setRowHidden(i, false);
                    }
                }else{
                    LogFile::writeToLog(QString("GenericTableWidget::setTypeFilter(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
                }
            }
        }
    }else{
        for (auto i = 1; i < table->rowCount(); i++){
            if (table->isRowHidden(i)){
                table->setRowHidden(i, false);
            }
        }
    }
}

void GenericTableWidget::setTypeFilter(const QString &typeallowed, const QStringList &typesdisallowed){
    if (typeallowed != ""){
        onlyTypeAllowed = typeallowed;
        typesDisallowed = QStringList();
        QTableWidgetItem *item = nullptr;
        if (table->columnCount() == 2 && !table->isColumnHidden(1)){
            for (auto i = 1; i < table->rowCount(); i++){
                item = table->item(i, 1);
                if (item){
                    if (item->text() != typeallowed){
                        table->hideRow(i);
                    }else{
                        table->setRowHidden(i, false);
                    }
                }else{
                    LogFile::writeToLog(QString("GenericTableWidget::setTypeFilter(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
                }
            }
        }
    }else if (!typesdisallowed.isEmpty()){
        onlyTypeAllowed = "";
        typesDisallowed = typesdisallowed;
        QTableWidgetItem *item = nullptr;
        if (table->columnCount() == 2 && !table->isColumnHidden(1)){
            for (auto i = 1; i < table->rowCount(); i++){
                item = table->item(i, 1);
                if (item){
                    if (typesdisallowed.contains(item->text())){
                        table->hideRow(i);
                    }else{
                        table->setRowHidden(i, false);
                    }
                }else{
                    LogFile::writeToLog(QString("GenericTableWidget::setTypeFilter(): \nMissing table widget item for row "+QString::number(i)+"!!!"));
                }
            }
        }
    }else{
        onlyTypeAllowed = "";
        typesDisallowed = QStringList();
        for (auto i = 1; i < table->rowCount(); i++){
            if (table->isRowHidden(i)){
                table->setRowHidden(i, false);
            }
        }
    }
}

void GenericTableWidget::itemSelected(){
    QString name;
    int row = table->currentRow();
    if (row != lastSelectedRow){
        if (table->item(row, 0)){
            name = table->item(row, 0)->text();
        }
        lastSelectedRow = row;
        emit elementSelected(row, name);
        hide();
    }
}

void GenericTableWidget::itemSelectedAt(int row, int ){
    QString name;
    if (row != lastSelectedRow){
        if (table->item(row, 0)){
            name = table->item(row, 0)->text();
        }
        lastSelectedRow = row;
        emit elementSelected(row, name);
        hide();
    }
}

namespace UIHelper{

void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2, TableWidget *table){
    (table->item(row, 0)) ? table->item(row, 0)->setText(name) : table->setItem(row, 0, new TableWidgetItem(name));
    (table->item(row, 1)) ? table->item(row, 1)->setText(classname) : table->setItem(row, 1, new TableWidgetItem(classname, Qt::AlignCenter));
    (table->item(row, 2)) ? table->item(row, 2)->setText(bind) : table->setItem(row, 2, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    (table->item(row, 3)) ? table->item(row, 3)->setText(value) : table->setItem(row, 3, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
}

void setBinding(int index, int row, int column, const QString & variableName, const QString & path, hkVariableType type, bool isProperty, TableWidget *table, HkDynamicObject *bsData){
    auto checkitem = [&](const QString & text){
        auto tableitem = table->item(row, column);
        (tableitem) ? tableitem->setText(text) : LogFile::writeToLog("setBinding(): 'tableitem' is nullptr!!");
    };
    if (bsData){
        auto varBind = bsData->getVariableBindingSetData();
        if (!index){
            varBind->removeBinding(path);
            if (!varBind->getNumberOfBindings()){
                static_cast<HkDynamicObject *>(bsData)->getVariableBindingSet() = HkxSharedPtr();
                static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
            }
            checkitem(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->getVariableBindingSet() = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    LogFile::writeToLog("setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    LogFile::writeToLog("setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            checkitem(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        LogFile::writeToLog("setBinding(): The 'bsData' pointer is nullptr!!");
    }
}

void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path, TableWidget *table, HkxObject *bsData){
    QString varName;
    auto settextitem = [&](){
        auto tableitem = table->item(row, column);
        (tableitem) ? tableitem->setText(BINDING_ITEM_LABEL+varName) : LogFile::writeToLog("loadBinding(): 'tableitem' is nullptr!!");
    };
    if (bsData){
        if (varBind){
            auto index = varBind->getVariableIndexOfBinding(path);
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    auto tableitem = table->item(row, column);
                    (tableitem) ? tableitem->setCheckState(Qt::Checked) : LogFile::writeToLog("loadBinding(): 'tableitem' is nullptr!!");
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            (varName == "") ? varName = "NONE" : NULL;
            settextitem();
        }else{
            settextitem();
        }
    }else{
        LogFile::writeToLog("loadBinding(): The data is nullptr!!");
    }
}

void setGenerator(int index, const QString &name, DataIconManager *dynobj, hkbGenerator *child, HkxSignature sig, HkxObject::HkxType type, TableWidget *table, BehaviorGraphView *behaviorView, int row, int column){
    if (table){
        if (dynobj){
            auto indexOfGenerator = dynobj->getIndexOfObj(static_cast<DataIconManager*>(child));
            if (behaviorView){
                auto ptr = static_cast<BehaviorFile *>(dynobj->getParentFile())->getGeneratorDataAt(index - 1);
                auto signature = ptr->getSignature();
                if (ptr){
                    if (name != ptr->getName()){
                        LogFile::writeToLog("setGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                    }else if ((signature == NULL_SIGNATURE && signature != sig) || (type != HkxObject::TYPE_GENERATOR)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nThe selected object is an incorrect type!!!");
                    }else if (ptr == dynobj || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(child), 0, ptr, false)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    }
                }else{
                    auto item = behaviorView->getSelectedItem();
                    if (item){
                        behaviorView->removeItemFromGraph(item->getChildWithData(static_cast<DataIconManager*>(child)), indexOfGenerator);
                    }else{
                        LogFile::writeToLog("setGenerator(): The selected icon is nullptr!!");
                        return;
                    }
                }
                behaviorView->removeGeneratorData();
                auto tableitem = table->item(row, column);
                (tableitem) ? tableitem->setText(name) : LogFile::writeToLog("setGenerator(): The 'tableitem' pointer is nullptr!!");
                dynobj->setIsFileChanged(true);
            }else{
                LogFile::writeToLog("setGenerator(): The 'behaviorView' pointer is nullptr!!");
            }
        }else{
            LogFile::writeToLog("setGenerator(): The 'generator' pointer is nullptr!!");
        }
    }else{
        LogFile::writeToLog("setGenerator(): The 'table' pointer is nullptr!!");
    }
}

void setModifier(int index, const QString &name, DataIconManager *dynobj, hkbModifier *child, HkxSignature sig, HkxObject::HkxType type, TableWidget *table, BehaviorGraphView *behaviorView, int row, int column){
    if (table){
        if (dynobj){
            auto indexofmodifier = dynobj->getIndexOfObj(static_cast<DataIconManager*>(child));
            if (behaviorView){
                auto ptr = static_cast<BehaviorFile *>(dynobj->getParentFile())->getModifierDataAt(index - 1);
                auto signature = ptr->getSignature();
                if (ptr){
                    if (name != ptr->getName()){
                        LogFile::writeToLog("setModifier():The name of the selected object does not match it's name in the object selection table!!!");
                    }else if ((signature == NULL_SIGNATURE && signature != sig) || (type != HkxObject::TYPE_MODIFIER)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nThe selected object is an incorrect type!!!");
                    }else if (ptr == dynobj || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(child), 0, ptr, false)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    }
                }else{
                    auto item = behaviorView->getSelectedItem();
                    if (item){
                        behaviorView->removeItemFromGraph(item->getChildWithData(static_cast<DataIconManager*>(child)), indexofmodifier);
                    }else{
                        LogFile::writeToLog("setModifier(): The selected icon is nullptr!!");
                        return;
                    }
                }
                behaviorView->removeModifierData();
                auto tableitem = table->item(row, column);
                (tableitem) ? tableitem->setText(name) : LogFile::writeToLog("setModifier(): The 'tableitem' pointer is nullptr!!");
                dynobj->setIsFileChanged(true);
            }else{
                LogFile::writeToLog("setModifier(): The 'behaviorView' pointer is nullptr!!");
            }
        }else{
            LogFile::writeToLog("setModifier(): The 'modifier' pointer is nullptr!!");
        }
    }else{
        LogFile::writeToLog("setModifier(): The 'table' pointer is nullptr!!");
    }
}


}
