#include "bsragdollcontactlistenermodifierui.h"


#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/hkxclasses/behavior/modifiers/BSRagdollContactListenerModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/boneindexarrayui.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 5

#define NAME_ROW 0
#define ENABLE_ROW 1
#define CONTACT_EVENT_ID_ROW 2
#define CONTACT_EVENT_PAYLOAD_ROW 3
#define BONES_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSRagdollContactListenerModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSRagdollContactListenerModifierUI::BSRagdollContactListenerModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox),
      table(new TableWidget(QColor(Qt::white))),
      boneIndexUI(new BoneIndexArrayUI),
      name(new LineEdit),
      enable(new CheckBox),
      contactEventPayload(new QLineEdit),
      bones(new CheckButtonCombo("Click to edit"))
{
    groupBox->setTitle("BSRagdollContactListenerModifierUI");
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
    table->setItem(CONTACT_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("contactEventId"));
    table->setItem(CONTACT_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(CONTACT_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(CONTACT_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(CONTACT_EVENT_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("contactEventPayload"));
    table->setItem(CONTACT_EVENT_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(CONTACT_EVENT_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(CONTACT_EVENT_PAYLOAD_ROW, VALUE_COLUMN, contactEventPayload);
    table->setItem(BONES_ROW, NAME_COLUMN, new TableWidgetItem("bones"));
    table->setItem(BONES_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneIndexArray", Qt::AlignCenter));
    table->setItem(BONES_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(BONES_ROW, VALUE_COLUMN, bones);
    topLyt->addWidget(table, 0, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneIndexUI);
}

void BSRagdollContactListenerModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(contactEventPayload, SIGNAL(editingFinished()), this, SLOT(setContactEventPayload()), Qt::UniqueConnection);
    connect(bones, SIGNAL(pressed()), this, SLOT(viewBones()), Qt::UniqueConnection);
    connect(bones, SIGNAL(enabled(bool)), this, SLOT(toggleBones(bool)), Qt::UniqueConnection);
    connect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSRagdollContactListenerModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(contactEventPayload, SIGNAL(editingFinished()), this, SLOT(setContactEventPayload()));
    disconnect(bones, SIGNAL(pressed()), this, SLOT(viewBones()));
    disconnect(bones, SIGNAL(enabled(bool)), this, SLOT(toggleBones(bool)));
    disconnect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSRagdollContactListenerModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events, GenericTableWidget *ragdollBones){
    if (variables && properties && events && ragdollBones){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(ragdollBones, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(setContactEventId(int,QString)), Qt::UniqueConnection);
        connect(ragdollBones, SIGNAL(elementSelected(int,QString)), boneIndexUI, SLOT(setRagdollBone(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(boneIndexUI, SIGNAL(viewRagdollBones(int)), ragdollBones, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_RAGDOLL_CONTACT_LISTENER_MODIFIER){
            bsData = static_cast<BSRagdollContactListenerModifier *>(data);
            hkbVariableBindingSet *varBind = NULL;
            hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->contactEvent.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->contactEvent.id);
            if (text != ""){
                table->item(CONTACT_EVENT_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(CONTACT_EVENT_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload){
                contactEventPayload->setText(payload->data);
            }else{
                contactEventPayload->setText("");
            }
            if (bsData->bones.data()){
                bones->setChecked(true);
                bones->setText("Click to Edit");
            }else{
                bones->setChecked(false);
                bones->setText("NULL");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            (qFatal("BSRagdollContactListenerModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSRagdollContactListenerModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::setName(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::setContactEventId(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->contactEvent.id != index){
            bsData->contactEvent.id = index;
            table->item(CONTACT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::setcontactEventId(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::setContactEventPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->contactEvent.payload.data());
        if (contactEventPayload->text() != ""){
            if (payload){
                payload->data = contactEventPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), contactEventPayload->text());
                bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->contactEvent.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->contactEvent.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::setcontactEventPayload(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::toggleBones(bool enable){
    if (bsData){
        if (!enable){
            bsData->bones = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (!bsData->bones.data()){
            hkbBoneIndexArray *indices = new hkbBoneIndexArray(bsData->getParentFile());
            bsData->getParentFile()->addObjectToFile(indices, -1);
            bsData->bones = HkxSharedPtr(indices);
            bones->setText("Click to Edit");
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::toggleBones(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::viewBones(){
    if (bsData){
        boneIndexUI->loadData(bsData->bones.data());
        setCurrentIndex(BONE_INDEX_WIDGET);
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::viewBones(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::viewSelected(int row, int column){
    if (bsData){
        bool isProperty = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enable");
                break;
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == CONTACT_EVENT_ID_ROW){
            emit viewEvents(bsData->contactEvent.id + 1);
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        (qFatal("BSRagdollContactListenerModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->contactEvent.id){
            table->item(CONTACT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::eventRenamed(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

bool BSRagdollContactListenerModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    (qFatal("BSRagdollContactListenerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("BSRagdollContactListenerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSRagdollContactListenerModifierUI::setBindingVariable(int index, const QString &name){
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
        (qFatal("BSRagdollContactListenerModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSRagdollContactListenerModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("BSRagdollContactListenerModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("BSRagdollContactListenerModifierUI::loadBinding(): The data is NULL!!"));
    }
}

