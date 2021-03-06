#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtGui>
#include "combination.h"
#include "element.h"
#include "elementItem.h"
#include "graphics.h"
#include "notification.h"
#include "scrollbar.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    void showDialog(dialogBase *d);

private:
    QString username;

    elementItems known_elements;
    QList<int> known_combinations;
    elementItems workspace;

    // 工作区
    QGraphicsScene *ws_scene;
    workspaceGraphicsItem *ws_parent;
    QWidget *ws_pgv;
    graphicsViewBase *ws_gv;
    QGraphicsBlurEffect *ws_blur;
    QPropertyAnimation *ws_blur_a;

    // 抽屉
    drawerGraphicsItem *dwr;
    scrollBar *dwr_sb;
    QGraphicsRectItem *dwr_parent;
    QPropertyAnimation *dwr_a;
    textItem *dwr_avas;

    // 对话框
    dialogBase *dialog;
    QPropertyAnimation *dialog_a;
    mouseShield *shield;

    // "删除"
    imageItem *deleteItem;
    QGraphicsDropShadowEffect *deleteGlow;
    QPropertyAnimation *deleteAnimation;
    QPropertyAnimation *deleteGlowAnimation;

    // 通知
    QQueue<notification*> notificationQueue;
    bool notificationShowing;

    elementItem* addElementToWorkspace(const element &e);
    elementItem* addElementToDrawer(const element &e);

    void fadeInAndAdd(elementItem *item);
    void fadeOutAndRemove(elementItem *item);

    void initializeDialog();
    void initializeWorkspace();
    void initializeDrawer();
    void initializeAchievements();
    void loadGame(const QString &username);
    void saveGame();

    void showNotification(const QImage &icon, const QString &line1, const QString &line2, const int &ap);
    void updateDwr_avas();
    void setScrollBars();

private slots:
    void addElementToWorkspace(QGraphicsSceneMouseEvent *e);
    void achievementAchieved();
    void notificationClose();

    void elementItem_copyElementItem();
    void elementItems_mousePressed(QGraphicsSceneMouseEvent *);
    void elementItems_mouseRightButtonPressed(QGraphicsSceneMouseEvent *);
    void elementItems_mouseReleased(QGraphicsSceneMouseEvent *);
    void elementItems_posChanged();

    void fadeIn_frameChanged(int frame);
    void fadeOut_finished();
    void fadeOut_frameChanged(int frame);

    void ws_parent_mouseDoubleClicked(QGraphicsSceneMouseEvent *e);
    void dwr_hoverEnter(QGraphicsSceneHoverEvent *);
    void dwr_hoverLeave(QGraphicsSceneHoverEvent *);
    void dwr_sb_valueChanged();

    void dialog_resized(QResizeEvent *);
    void shield_mouseReleased();
    void dialog_a_finished();

protected:
    void resizeEvent(QResizeEvent *);
};

class QTimeLineE : public QTimeLine
{
    Q_OBJECT
public:
    explicit QTimeLineE(int duration = 1000, QObject *parent = 0) : QTimeLine(duration, parent) { }
    elementItem *target;
};

#endif // GAMEWIDGET_H
