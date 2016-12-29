#ifndef BLENDERGENERATORUI_H
#define BLENDERGENERATORUI_H

#include <QStackedWidget>

class QuadVariableWidget;
class TableWidget;
class hkbBlenderGenerator;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class QStackedLayout;
class SpinBox;
class QPushButton;
class QGroupBox;
class BlenderGeneratorChildUI;
class QLineEdit;
class ComboBox;

class BlenderGeneratorUI: public QStackedWidget
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    BlenderGeneratorUI();
    virtual ~BlenderGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setReferencePoseWeightThreshold();
    void setReferencePoseWeightThresholdBind(int index);
    void setBlendParameter();
    void setBlendParameterBind(int index);
    void setMinCyclicBlendParameter();
    void setMinCyclicBlendParameterBind(int index);
    void setMaxCyclicBlendParameter();
    void setMaxCyclicBlendParameterBind(int index);
    void setIndexOfSyncMasterChild();
    void setIndexOfSyncMasterChildBind(int index);
    void setFlagSync();
    void setFlagSmoothGeneratorWeights();
    void setFlagDontDeactivateChildrenWithZeroWeights();
    void setFlagParametricBlend();
    void setFlagIsParametricBlendCyclic();
    void setFlagForceDensePose();
    void setSubtractLastChild();
    void setSubtractLastChild(int index);
    void addChild();
    void removeSelectedChild();
    void viewSelectedChild(int row, int column);
    void returnToWidget();
private:
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    void loadComboBoxes();
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };

    static QStringList headerLabels;
    hkbBlenderGenerator *bsData;
    QGroupBox *groupBox;
    BlenderGeneratorChildUI *childUI;
    QGridLayout *topLyt;
    QPushButton *addChildPB;
    QPushButton *removeChildPB;
    TableWidget *table;
    QLineEdit *name;
    DoubleSpinBox *referencePoseWeightThreshold;
    ComboBox *referencePoseWeightThresholdBind;
    DoubleSpinBox *blendParameter;
    ComboBox *blendParameterBind;
    DoubleSpinBox *minCyclicBlendParameter;
    ComboBox *minCyclicBlendParameterBind;
    DoubleSpinBox *maxCyclicBlendParameter;
    ComboBox *maxCyclicBlendParameterBind;
    SpinBox *indexOfSyncMasterChild;
    ComboBox *indexOfSyncMasterChildBind;
    QCheckBox *flagSync;
    QCheckBox *flagSmoothGeneratorWeights;
    QCheckBox *flagDontDeactivateChildrenWithZeroWeights;
    QCheckBox *flagParametricBlend;
    QCheckBox *flagIsParametricBlendCyclic;
    QCheckBox *flagForceDensePose;
    QCheckBox *subtractLastChild;
    ComboBox *subtractLastChildBind;
};

#endif // BLENDERGENERATORUI_H
