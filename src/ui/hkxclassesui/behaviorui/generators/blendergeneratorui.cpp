#include "blendergeneratorui.h"
#include "blendergeneratorchildui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbblendergenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>

#define BASE_NUMBER_OF_ROWS 13

/*
 * BlenderGeneratorUI
 */

QStringList BlenderGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BlenderGeneratorUI::BlenderGeneratorUI()
    : bsData(NULL),
      groupBox(new QGroupBox("hkbBlenderGenerator")),
      childUI(new BlenderGeneratorChildUI),
      topLyt(new QGridLayout),
      addChildPB(new QPushButton("Add leg")),
      removeChildPB(new QPushButton("Remove selected leg")),
      table(new TableWidget),
      name(new QLineEdit),
      referencePoseWeightThreshold(new DoubleSpinBox),
      referencePoseWeightThresholdBind(new ComboBox),
      blendParameter(new DoubleSpinBox),
      blendParameterBind(new ComboBox),
      minCyclicBlendParameter(new DoubleSpinBox),
      minCyclicBlendParameterBind(new ComboBox),
      maxCyclicBlendParameter(new DoubleSpinBox),
      maxCyclicBlendParameterBind(new ComboBox),
      indexOfSyncMasterChild(new SpinBox),
      indexOfSyncMasterChildBind(new ComboBox),
      flagSync(new QCheckBox),
      flagSmoothGeneratorWeights(new QCheckBox),
      flagDontDeactivateChildrenWithZeroWeights(new QCheckBox),
      flagParametricBlend(new QCheckBox),
      flagIsParametricBlendCyclic(new QCheckBox),
      flagForceDensePose(new QCheckBox),
      subtractLastChild(new QCheckBox),
      subtractLastChildBind(new ComboBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(0, 0, new QTableWidgetItem("name"));
    table->setItem(0, 1, new QTableWidgetItem("hkStringPtr"));
    table->setItem(0, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(0, 3, name);
    table->setItem(1, 0, new QTableWidgetItem("referencePoseWeightThreshold"));
    table->setItem(1, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(1, 2, referencePoseWeightThresholdBind);
    table->setCellWidget(1, 3, referencePoseWeightThreshold);
    table->setItem(2, 0, new QTableWidgetItem("blendParameter"));
    table->setItem(2, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(2, 2, blendParameterBind);
    table->setCellWidget(2, 3, blendParameter);
    table->setItem(3, 0, new QTableWidgetItem("minCyclicBlendParameter"));
    table->setItem(3, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(3, 2, minCyclicBlendParameterBind);
    table->setCellWidget(3, 3, minCyclicBlendParameter);
    table->setItem(4, 0, new QTableWidgetItem("maxCyclicBlendParameter"));
    table->setItem(4, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(4, 2, maxCyclicBlendParameterBind);
    table->setCellWidget(4, 3, maxCyclicBlendParameter);
    table->setItem(5, 0, new QTableWidgetItem("indexOfSyncMasterChild"));
    table->setItem(5, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(5, 2, indexOfSyncMasterChildBind);
    table->setCellWidget(5, 3, indexOfSyncMasterChild);
    table->setItem(6, 0, new QTableWidgetItem("flagSync"));
    table->setItem(6, 1, new QTableWidgetItem("hkInt16"));
    table->setCellWidget(6, 2, flagSync);
    table->setItem(7, 0, new QTableWidgetItem("flagSmoothGeneratorWeights"));
    table->setItem(7, 1, new QTableWidgetItem("hkInt16"));
    table->setCellWidget(7, 2, flagSmoothGeneratorWeights);
    table->setItem(8, 0, new QTableWidgetItem("flagDontDeactivateChildrenWithZeroWeights"));
    table->setItem(8, 1, new QTableWidgetItem("hkInt16"));
    table->setCellWidget(8, 2, flagDontDeactivateChildrenWithZeroWeights);
    table->setItem(9, 0, new QTableWidgetItem("flagParametricBlend"));
    table->setItem(9, 1, new QTableWidgetItem("hkInt16"));
    table->setCellWidget(9, 2, flagParametricBlend);
    table->setItem(10, 0, new QTableWidgetItem("flagIsParametricBlendCyclic"));
    table->setItem(10, 1, new QTableWidgetItem("hkInt16"));
    table->setCellWidget(10, 2, flagIsParametricBlendCyclic);
    table->setItem(11, 0, new QTableWidgetItem("flagForceDensePose"));
    table->setItem(11, 1, new QTableWidgetItem("hkInt16"));
    table->setCellWidget(11, 2, flagForceDensePose);
    table->setItem(12, 0, new QTableWidgetItem("subtractLastChild"));
    table->setItem(12, 1, new QTableWidgetItem("hkBool"));
    table->setCellWidget(12, 2, subtractLastChildBind);
    table->setCellWidget(12, 3, subtractLastChild);
    topLyt->addWidget(addChildPB, 0, 0, 1, 1);
    topLyt->addWidget(removeChildPB, 0, 2, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    addWidget(groupBox);
    addWidget(childUI);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(referencePoseWeightThreshold, SIGNAL(editingFinished()), this, SLOT(setReferencePoseWeightThreshold()));
    connect(referencePoseWeightThresholdBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setReferencePoseWeightThresholdBind(int)));
    connect(blendParameter, SIGNAL(editingFinished()), this, SLOT(setBlendParameter()));
    connect(blendParameterBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendParameterBind(int)));
    connect(minCyclicBlendParameter, SIGNAL(editingFinished()), this, SLOT(setMinCyclicBlendParameter()));
    connect(minCyclicBlendParameterBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setMinCyclicBlendParameterBind(int)));
    connect(maxCyclicBlendParameter, SIGNAL(editingFinished()), this, SLOT(setMaxCyclicBlendParameter()));
    connect(maxCyclicBlendParameterBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setMaxCyclicBlendParameterBind(int)));
    connect(indexOfSyncMasterChild, SIGNAL(editingFinished()), this, SLOT(setIndexOfSyncMasterChild()));
    connect(indexOfSyncMasterChildBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setIndexOfSyncMasterChildBind(int)));
    connect(flagSync, SIGNAL(released()), this, SLOT(setFlagSync()));
    connect(flagSmoothGeneratorWeights, SIGNAL(released()), this, SLOT(setFlagSmoothGeneratorWeights()));
    connect(flagDontDeactivateChildrenWithZeroWeights, SIGNAL(released()), this, SLOT(setFlagDontDeactivateChildrenWithZeroWeights()));
    connect(flagParametricBlend, SIGNAL(released()), this, SLOT(setFlagParametricBlend()));
    connect(flagIsParametricBlendCyclic, SIGNAL(released()), this, SLOT(setFlagIsParametricBlendCyclic()));
    connect(subtractLastChild, SIGNAL(released()), this, SLOT(setSubtractLastChild()));
    connect(subtractLastChildBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setSubtractLastChild(int)));
    connect(addChildPB, SIGNAL(released()), this, SLOT(addChild()));
    connect(removeChildPB, SIGNAL(released()), this, SLOT(removeSelectedChild()));
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    connect(childUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void BlenderGeneratorUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_FOOT_IK_DRIVER_INFO){
        bsData = static_cast<hkbFootIkDriverInfo *>(data);
        name->setText(bsData->name);
        referencePoseWeightThreshold->setValue(bsData->referencePoseWeightThreshold);
        blendParameter->setValue(bsData->blendParameter);
        minCyclicBlendParameter->setValue(bsData->minCyclicBlendParameter);
        maxCyclicBlendParameter->setValue(bsData->maxCyclicBlendParameter);
        indexOfSyncMasterChild->setValue(bsData->indexOfSyncMasterChild);
        QStringList flags = bsData->flags.split("|");
        if (flags.isEmpty()){
            if (bsData->flags == "FLAG_SYNC"){
                flagSync->setChecked(true);
            }else if (bsData->flags == "FLAG_SMOOTH_GENERATOR_WEIGHTS"){
                flagSmoothGeneratorWeights->setChecked(true);
            }else if (bsData->flags == "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS"){
                flagDontDeactivateChildrenWithZeroWeights->setChecked(true);
            }else if (bsData->flags == "FLAG_PARAMETRIC_BLEND"){
                flagParametricBlend->setChecked(true);
            }else if (bsData->flags == "FLAG_IS_PARAMETRIC_BLEND_CYCLIC"){
                flagIsParametricBlendCyclic->setChecked(true);
            }else if (bsData->flags == "FLAG_FORCE_DENSE_POSE"){
                flagForceDensePose->setChecked(true);
            }
        }else{
            for (int i = 0; i < flags.size(); i++){
                if (flags.at(i) == "FLAG_SYNC"){
                    flagSync->setChecked(true);
                }else if (flags.at(i) == "FLAG_SMOOTH_GENERATOR_WEIGHTS"){
                    flagSmoothGeneratorWeights->setChecked(true);
                }else if (flags.at(i) == "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS"){
                    flagDontDeactivateChildrenWithZeroWeights->setChecked(true);
                }else if (flags.at(i) == "FLAG_PARAMETRIC_BLEND"){
                    flagParametricBlend->setChecked(true);
                }else if (flags.at(i) == "FLAG_IS_PARAMETRIC_BLEND_CYCLIC"){
                    flagIsParametricBlendCyclic->setChecked(true);
                }else if (flags.at(i) == "FLAG_FORCE_DENSE_POSE"){
                    flagForceDensePose->setChecked(true);
                }
            }
        }
        subtractLastChild->setChecked(bsData->subtractLastChild);
        for (int i = 0, k; i < bsData->legs.size(); i++){
            k = i + BASE_NUMBER_OF_ROWS;
            if (k >= table->rowCount()){
                table->setRowCount(table->rowCount() + 1);
                table->setItem(k, 0, new QTableWidgetItem("Child "+QString::number(i)));
                table->setItem(k, 1, new QTableWidgetItem("hkbBlenderGeneratorChild"));
                table->setItem(k, 2, new QTableWidgetItem("Click to Edit"));
            }else{
                table->setRowHidden(k, false);
                table->item(k, 0)->setText("Child "+QString::number(i));
                table->item(k, 1)->setText("hkbBlenderGeneratorChild");
            }
        }
        for (int j = bsData->legs.size() + BASE_NUMBER_OF_ROWS; j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
    }
}

void BlenderGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->text();
        emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setReferencePoseWeightThreshold(){
    if (bsData){
        bsData->referencePoseWeightThreshold = referencePoseWeightThreshold->value();
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setBlendParameter(){
    if (bsData){
        bsData->blendParameter = blendParameter->value();
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setMinCyclicBlendParameter(){
    if (bsData){
        bsData->minCyclicBlendParameter = minCyclicBlendParameter->value();
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setMaxCyclicBlendParameter(){
    if (bsData){
        bsData->maxCyclicBlendParameter = maxCyclicBlendParameter->value();
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setIndexOfSyncMasterChild(){
    if (bsData){
        bsData->indexOfSyncMasterChild = indexOfSyncMasterChild->value();
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setFlagSync(){
    if (bsData){
        if (flagSync->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_SYNC");
            }else if (!bsData->flags.contains("FLAG_SYNC")){
                bsData->flags.append("|FLAG_SYNC");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_SYNC");
            }else{
                bsData->flags.remove("|FLAG_SYNC");
            }
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setFlagSmoothGeneratorWeights(){
    if (bsData){
        if (flagSmoothGeneratorWeights->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }else if (!bsData->flags.contains("FLAG_SMOOTH_GENERATOR_WEIGHTS")){
                bsData->flags.append("|FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }else{
                bsData->flags.remove("|FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(){
    if (bsData){
        if (flagDontDeactivateChildrenWithZeroWeights->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }else if (!bsData->flags.contains("FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS")){
                bsData->flags.append("|FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }else{
                bsData->flags.remove("|FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setFlagParametricBlend(){
    if (bsData){
        if (flagParametricBlend->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_PARAMETRIC_BLEND");
            }else if (!bsData->flags.contains("FLAG_PARAMETRIC_BLEND")){
                bsData->flags.append("|FLAG_PARAMETRIC_BLEND");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_PARAMETRIC_BLEND");
            }else{
                bsData->flags.remove("|FLAG_PARAMETRIC_BLEND");
            }
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setFlagIsParametricBlendCyclic(){
    if (bsData){
        if (flagIsParametricBlendCyclic->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }else if (!bsData->flags.contains("FLAG_IS_PARAMETRIC_BLEND_CYCLIC")){
                bsData->flags.append("|FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }else{
                bsData->flags.remove("|FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setFlagForceDensePose(){
    if (bsData){
        if (flagForceDensePose->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_FORCE_DENSE_POSE");
            }else if (!bsData->flags.contains("FLAG_FORCE_DENSE_POSE")){
                bsData->flags.append("|FLAG_FORCE_DENSE_POSE");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_FORCE_DENSE_POSE");
            }else{
                bsData->flags.remove("|FLAG_FORCE_DENSE_POSE");
            }
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::setSubtractLastChild(){
    if (bsData){
        bsData->subtractLastChild = subtractLastChild->isChecked();
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::addChild(){
    if (bsData){
        bsData->addLeg();
        int result = BASE_NUMBER_OF_ROWS + bsData->legs.size();
        if (result >= table->rowCount()){
            result--;
            table->setRowCount(table->rowCount() + 1);
            table->setItem(result, 0, new QTableWidgetItem("Child "+QString::number(bsData->legs.size() - 1)));
            table->setItem(result, 1, new QTableWidgetItem("hkbBlenderGeneratorChild"));
            table->setItem(result, 2, new QTableWidgetItem("Click to Edit"));
        }else{
            result--;
            table->setRowHidden(result, false);
            table->item(result, 0)->setText("Child "+QString::number(bsData->legs.size() - 1));
            table->item(result, 1)->setText("hkbBlenderGeneratorChild");
        }
        //behaviorView->toggleChanged(true);
    }
}

void BlenderGeneratorUI::removeSelectedChild(){
    int result = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData && result < bsData->legs.size()){
        bsData->removeLegAt(result);
        delete table->takeItem(table->currentRow(), 0);
        delete table->takeItem(table->currentRow(), 1);
        delete table->takeItem(table->currentRow(), 2);
        table->removeRow(table->currentRow());
    }
}

void BlenderGeneratorUI::viewSelectedChild(int row, int column){
    int result = row - BASE_NUMBER_OF_ROWS;
    if (bsData && bsData->children.size() > result && column == 2){
        childUI->loadData((hkbBlenderGeneratorChild *)bsData->children.at(result));
        setCurrentIndex(CHILD_WIDGET);
    }
}

void BlenderGeneratorUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void BlenderGeneratorUI::addVariableToLists(const QString & name){
    iStateToSetAsBind->insertItem(iStateToSetAsBind->count(), name);
    iPriorityBind->insertItem(iPriorityBind->count(), name);
}

void BlenderGeneratorUI::removeVariableFromLists(int index){
    index++;
    if (iStateToSetAsBind->currentIndex() == index){
        iStateToSetAsBind->setCurrentIndex(0);
    }
    iStateToSetAsBind->removeItem(index);
    if (iPriorityBind->currentIndex() == index){
        iPriorityBind->setCurrentIndex(0);
    }
    iPriorityBind->removeItem(index);
}

void BlenderGeneratorUI::renameVariableInLists(const QString & newName, int index){
    index++;
    iStateToSetAsBind->setItemText(index, newName);
    iPriorityBind->setItemText(index, newName);
}

