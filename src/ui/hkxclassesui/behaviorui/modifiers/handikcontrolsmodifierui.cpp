#include "handikcontrolsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/handui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbHandIkControlsModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 3

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ADD_HAND_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList HandIkControlsModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

HandIkControlsModifierUI::HandIkControlsModifierUI()
    : bsData(NULL),
      handUI(new HandUI),
      groupBox(new QGroupBox),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox)
{
    groupBox->setTitle("hkbHandIkControlsModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(ADD_HAND_ROW, NAME_COLUMN, new TableWidgetItem("Add Hand", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new Hand"));
    table->setItem(ADD_HAND_ROW, TYPE_COLUMN, new TableWidgetItem("hkHand", Qt::AlignCenter));
    table->setItem(ADD_HAND_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Hand", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected Hand"));
    table->setItem(ADD_HAND_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Hand", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected Hand"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(handUI);
    connectSignals();
}

void HandIkControlsModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(handUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(handUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void HandIkControlsModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(handUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(handUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void HandIkControlsModifierUI::addHand(){
    if (bsData){
        bsData->hands.append(hkbHandIkControlsModifier::hkHand());
        bsData->getParentFile()->setIsChanged(true);
        loadDynamicTableRows();
    }else{
        (qFatal("HandIkControlsModifierUI::addHand(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::removeHand(int index){
    if (bsData){
        if (!bsData->hands.isEmpty()){
            if (index < bsData->hands.size() && index >= 0){
                bsData->hands.removeAt(index);
            }else{
                (qWarning("HandIkControlsModifierUI::removeHand(): Invalid row index selected!!"));
                return;
            }
            bsData->getParentFile()->setIsChanged(true);
            loadDynamicTableRows();
        }else{
            (qWarning("HandIkControlsModifierUI::removeHand(): Ranges is empty!!"));
            return;
        }
    }else{
        (qFatal("HandIkControlsModifierUI::removeHand(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == HKB_HAND_IK_CONTROLS_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbHandIkControlsModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            (qFatal(QString("HandIkControlsModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("HandIkControlsModifierUI::loadData(): Attempting to load a null pointer!!"));
    }
    connectSignals();
}

void HandIkControlsModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_HAND_ROW + bsData->getNumberOfHands() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        for (int i = ADD_HAND_ROW + 1, j = 0; j < bsData->getNumberOfHands(); i++, j++){
            setRowItems(i, "Hand "+QString::number(j), "hkHand", "Remove", "Edit", "Double click to remove this Hand", "Double click to edit this Hand");
        }
    }else{
        (qFatal("HandIkControlsModifierUI::loadDynamicTableRows(): The data is NULL!!"));
    }
    //table->setSortingEnabled(true);
}

void HandIkControlsModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(classname, Qt::AlignCenter));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

bool HandIkControlsModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    (qFatal("HandIkControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("HandIkControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("HandIkControlsModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void HandIkControlsModifierUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case ENABLE_ROW:
            if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enable", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("HandIkControlsModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void HandIkControlsModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit modifierNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("HandIkControlsModifierUI::setName(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("HandIkControlsModifierUI::setEnable(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_HAND_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                }
            }
        }else if (row == ADD_HAND_ROW && column == NAME_COLUMN){
            addHand();
        }else if (row > ADD_HAND_ROW && row < ADD_HAND_ROW + bsData->getNumberOfHands() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfHands() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    handUI->loadData(((BehaviorFile *)(bsData->getParentFile())), &bsData->hands[result], bsData, result);
                    setCurrentIndex(HAND_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the Hand \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeHand(result);
                    }
                }
            }else{
                (qFatal("HandIkControlsModifierUI::viewSelectedChild(): Invalid index of range to view!!"));
            }
        }
    }else{
        (qFatal("HandIkControlsModifierUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case HAND_WIDGET:
        handUI->setBindingVariable(index, name);
        break;
    default:
        (qWarning("HandIkControlsModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!"));
    }
}

void HandIkControlsModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("HandIkControlsModifierUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void HandIkControlsModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            (qFatal("HandIkControlsModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("HandIkControlsModifierUI::loadBinding(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        (qFatal("HandIkControlsModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void HandIkControlsModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        (qWarning("HandIkControlsModifierUI::variableRenamed(): The new variable name is the empty string!!"));
    }
    if (bsData){
        index--;
        if (currentIndex() == MAIN_WIDGET){
            bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (bind){
                bindIndex = bind->getVariableIndexOfBinding("enable");
                if (bindIndex == index){
                    table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
                }
            }
        }else{
            handUI->variableRenamed(name, index);
        }
    }else{
        (qFatal("HandIkControlsModifierUI::variableRenamed(): The data is NULL!!"));
    }
}

