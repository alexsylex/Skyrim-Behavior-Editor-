#ifndef TREEGRAPHICSVIEW_H
#define TREEGRAPHICSVIEW_H

#include <QGraphicsView>

class QMenu;
class TreeGraphicsScene;
class DataIconManager;
class TreeGraphicsItem;

class TreeGraphicsView: public QGraphicsView
{
    Q_OBJECT
public:
    TreeGraphicsView(QMenu *menu);
    bool drawGraph(DataIconManager *rootData, bool allowDuplicates = false);
    TreeGraphicsItem * getSelectedItem() const;
    bool reconnectIcon(TreeGraphicsItem *oldIconParent, DataIconManager *dataToReplace, DataIconManager *replacementData);
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contractAllBranches();
    void expandAllBranches();
    void selectRoot();
    bool removeItemFromGraph(TreeGraphicsItem *item, int indexToRemove);
    TreeGraphicsItem * addItemToGraph(TreeGraphicsItem *selectedIcon, DataIconManager *data, int indexToInsert, bool inject = false, bool allowDuplicates = false);
private:
    QMenu *popUpMenu;
    TreeGraphicsScene *treeScene;
    const qreal minScale;
    const qreal maxScale;
    const qreal initScale;
    const qreal iconFocusScale;
    qreal currentScale;
    const qreal scaleUpFactor;
    const qreal scaleDownFactor;
};

#endif // TREEGRAPHICSVIEW_H