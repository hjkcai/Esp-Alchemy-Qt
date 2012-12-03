#ifndef __Alchemy__GameWidget__
#define __Alchemy__GameWidget__

#include <QtGui>
#include "combination.h"
#include "des.h"
#include "element.h"
#include "elementItem.h"
#include "graphics.h"
#include "inappdialog.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);

private:
    static const int drawerAnimationDuration = 200;
    bool drawerOpened;

    QString username;
    CDes des;

    elementItems known_elements;
    QList<int> known_combinations;
    elementItems workspace;

    QGraphicsScene *ws_scene;
    QGraphicsRectItem *ws_parent;
    QWidget *ws_pgv;
    graphicsViewBase *ws_gv;
    QPropertyAnimation *ws_a;

    drawerGraphicsScene *dwr_scene;
    QGraphicsRectItem *dwr_parent;
    QWidget *dwr_pgv;
    graphicsViewBase *dwr_gv;
    QPropertyAnimation *dwr_a;

    elementItem* addElementToWorkspace(const element &e);
    elementItem* addElementToDrawer(const element &e);

    void fadeInAndAdd(elementItem *item);
    void fadeOutAndRemove(elementItem *item);

    void initializeWorkspace();
    void initializeDrawer();
    void loadGame(const QString &username);
    void saveGame();

    void setScrollBars();

private slots:
    void addElementToWorkspace(QGraphicsSceneMouseEvent *e);

    void elementItems_mousePressed(QGraphicsSceneMouseEvent *);
    void elementItems_mouseReleased(QGraphicsSceneMouseEvent *);

    void fadeIn_frameChanged(int frame);
    void fadeOut_finished();
    void fadeOut_frameChanged(int frame);

    void ws_gv_mousePressed(QMouseEvent *);

    void showOrHideDrawer();

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void ShowDialog(InAppDialog *dialog);
};

class QTimeLineE : public QTimeLine
{
    Q_OBJECT
public:
    explicit QTimeLineE(int duration = 1000, QObject *parent = 0) : QTimeLine(duration, parent) { }
    elementItem *target;
};

#endif // __Alchemy__GameWidget__
