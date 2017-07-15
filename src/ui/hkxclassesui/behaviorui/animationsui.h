#ifndef ANIMATIONSUI_H
#define ANIMATIONSUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class TableWidget;
class QPushButton;
class QSignalMapper;
class hkbCharacterStringData;
class HkxObject;
class LineEdit;
class CheckBox;
class QStackedLayout;

class AnimationsUI: public QGroupBox
{
    Q_OBJECT
public:
    AnimationsUI(const QString & title);
    virtual ~AnimationsUI(){}
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data);
    void clear();
private slots:
    void addAnimation();
    void removeAnimation();
    void renameSelectedAnimation();
    void viewAnimation(int row, int column);
    void returnToTable();
signals:
    void animationNameChanged(const QString & newName, int index);
    void animationAdded(const QString & name);
    void animationRemoved(int index);
private:
    AnimationsUI& operator=(const AnimationsUI&);
    AnimationsUI(const AnimationsUI &);
private:
    enum View {
        TABLE_WIDGET = 0,
        ANIMATION_WIDGET = 1
    };
    static QStringList headerLabels;
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    hkbCharacterStringData *loadedData;
    TableWidget *table;
    QPushButton *addObjectPB;
    //QPushButton *removeObjectPB;
    QHBoxLayout *buttonLyt;
    LineEdit *animationName;
    TableWidget *animationNameWidget;
    QStackedLayout *stackLyt;
    QPushButton *returnPB;
};
#endif // ANIMATIONSUI_H