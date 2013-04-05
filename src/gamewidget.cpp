#include "achievement.h"
#include "configs.h"
#include "elementdetaildialog.h"
#include "gamewidget.h"
#include "global.h"
#include "introdialog.h"
#include <QGLWidget>
#include <cmath>
using namespace std;

const int drawerAnimationDuration = 200;
const int drawerItemOffsetX = 18;
const int drawerItemOffsetY = 5;
const QSize minSize = QSize(550, 400);

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(minSize);
    this->resize(minSize);
    this->setWindowTitle(tr("Alchemy"));
    this->setWindowIcon(QIcon(ResourcesDir.arg("icon.png")));

    initializeDialog();
    initializeWorkspace();
    initializeDrawer();
    initializeAchievements();

    // 默认调试用户
    loadGame("tester");

    for (int i = 0; i < 4; i++)
        addElementToWorkspace(element::allElements[i])->setPos(this->width() / 2 + (i - 2) * 64, this->height() / 2 - 84 / 2);

    showDialog(new introDialog());
}

void GameWidget::showDialog(dialogBase *d)
{
    dialog = d;
    dialog->setParentItem(shield);
    ws_scene->addItem(shield);
    dialog_resized(NULL);

    dialog_a->setDirection(QPropertyAnimation::Forward);
    dialog_a->start();
    ws_blur_a->setDirection(QPropertyAnimation::Forward);
    ws_blur_a->start();
}

elementItem* GameWidget::addElementToWorkspace(const element &e)
{
    elementItem *ei = new elementItem(e);
    ei->setZValue(workspace.length());
    connect(ei, SIGNAL(copyElementItem()), this, SLOT(elementItem_copyElementItem()));
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mousePressed(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mouseRightButtonPressed(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(mouseReleased(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mouseReleased(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(xChanged()), this, SLOT(elementItems_posChanged()));
    connect(ei, SIGNAL(yChanged()), this, SLOT(elementItems_posChanged()));

    workspace << ei;
    fadeInAndAdd(ei);

    return ei;
}

void GameWidget::addElementToWorkspace(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        elementItem *ei = qobject_cast<elementItem*>(sender());
        addElementToWorkspace(ei->base())->setPos(ei->mapToScene(0, 0));
    }
}

void GameWidget::achievementAchieved()
{
    qDebug() << qobject_cast<achievement*>(sender())->name();
}

elementItem* GameWidget::addElementToDrawer(const element &e)
{
    elementItem *ei = new elementItem(e);
    ei->setDrawerStyle(true);
    ei->setDrag(false);
    ei->setParentItem(dwr_parent);
    ei->setZValue(1);
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(addElementToWorkspace(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mouseRightButtonPressed(QGraphicsSceneMouseEvent*)));

    if (known_elements.length() == 0)
    {
        ei->setPos(drawerItemOffsetX, drawerItemOffsetY);
        known_elements << ei;
    }
    else
    {
        int i;
        for (i = 0; i < known_elements.length(); i++)
        {
            if (e.id().compare(known_elements[i]->base().id()) < 0)
            {
                // 插入并调整其它项的位置
                known_elements.insert(i, ei);
                for (int j = i; j < known_elements.length(); j++)
                    known_elements[j]->setPos(j % 4 * 64 + drawerItemOffsetX, j / 4 * 84 + drawerItemOffsetY);

                break;
            }
        }

        if (i == known_elements.length())
        {
            ei->setPos(known_elements.length() % 4 * 64 + drawerItemOffsetX, known_elements.length() / 4 * 84 + drawerItemOffsetY);
            known_elements << ei;
        }
    }

    updateDwr_avas();
    return ei;
}

void GameWidget::fadeInAndAdd(elementItem *item)
{
    item->setOpacity(0);
    item->setParentItem(ws_parent);

    QTimeLineE *tl = new QTimeLineE(AnimationDuration, this);
    tl->target = item;
    tl->setFrameRange(0, AnimationDuration);
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(fadeIn_frameChanged(int)));

    tl->start();
}

void GameWidget::fadeOutAndRemove(elementItem *item)
{
    QTimeLineE *tl = new QTimeLineE(AnimationDuration);
    tl->target = item;
    tl->setFrameRange(0, AnimationDuration);
    connect(tl, SIGNAL(finished()), this, SLOT(fadeOut_finished()));
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(fadeOut_frameChanged(int)));

    tl->start();
}

void GameWidget::initializeDialog()
{
    dialog = NULL;

    shield = new mouseShield();
    shield->setSize(this->size());
    connect(shield, SIGNAL(mouseReleased()), this, SLOT(shield_mouseReleased()));

    dialog_a = new QPropertyAnimation(shield, "opacity", this);
    dialog_a->setStartValue(0);
    dialog_a->setEndValue(1);
    dialog_a->setDuration(AnimationDuration);
    connect(dialog_a, SIGNAL(finished()), this, SLOT(dialog_a_finished()));
}

void GameWidget::initializeWorkspace()
{
    ws_scene = new QGraphicsScene();
    ws_parent = new workspaceGraphicsItem();
    ws_parent->setPos(0, 0);
    ws_parent->setSize(this->width(), this->height());
    connect(ws_parent, SIGNAL(mouseDoubleClicked(QGraphicsSceneMouseEvent*)), this, SLOT(ws_parent_mouseDoubleClicked(QGraphicsSceneMouseEvent*)));
    ws_scene->addItem(ws_parent);

    ws_pgv = new QWidget(this);
    ws_pgv->setGeometry(0, 0, this->width(), this->height());

    ws_gv = new graphicsViewBase(ws_scene, ws_pgv);
    ws_gv->setMouseTracking(true);
    ws_gv->setGeometry(-1, -1, this->width() + 2, this->height() + 2);
    ws_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ws_gv->setRenderHints(QPainter::Antialiasing);
    ws_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ws_gv->setViewport(new QGLWidget(this));    // OpenGL绘图
    //ws_gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    ws_blur = new QGraphicsBlurEffect(this);
    ws_blur->setBlurRadius(0);
    ws_parent->setGraphicsEffect(ws_blur);

    ws_blur_a = new QPropertyAnimation(ws_blur, "blurRadius", this);
    ws_blur_a->setStartValue(0);
    ws_blur_a->setEndValue(5);
    ws_blur_a->setDuration(AnimationDuration);

    // "删除"有关的对象

    deleteItem = new imageItem(QImage(ResourcesDir.arg("trashcan.png")), ws_parent);
    deleteItem->setPaintMode(imageItem::centerScale);
    deleteItem->setPos(-48, 0);
    deleteItem->setSize(48, 48);

    deleteGlow = new QGraphicsDropShadowEffect(deleteItem);
    deleteGlow->setBlurRadius(15);
    deleteGlow->setColor(QColor(255, 0, 0, 0));
    deleteGlow->setOffset(0);
    deleteItem->setGraphicsEffect(deleteGlow);

    deleteAnimation = new QPropertyAnimation(deleteItem, "pos", deleteItem);
    deleteAnimation->setDuration(AnimationDuration);
    deleteAnimation->setEasingCurve(QEasingCurve::InQuad);

    deleteGlowAnimation = new QPropertyAnimation(deleteGlow, "color", deleteGlow);
    deleteGlowAnimation->setDuration(200);
    deleteGlowAnimation->setStartValue(QColor(255, 0, 0, 0));
    deleteGlowAnimation->setEndValue(QColor(255, 0, 0));
}

void GameWidget::initializeDrawer()
{
    dwr = new drawerGraphicsItem();
    dwr->setPos(this->width() - 5, 0);
    dwr->updateBuffer(QSizeF(300, this->height()));
    connect(dwr, SIGNAL(hoverEnter(QGraphicsSceneHoverEvent*)), this, SLOT(dwr_hoverEnter(QGraphicsSceneHoverEvent*)));
    connect(dwr, SIGNAL(hoverLeave(QGraphicsSceneHoverEvent*)), this, SLOT(dwr_hoverLeave(QGraphicsSceneHoverEvent*)));
    ws_scene->addItem(dwr);

    dwr_avas = new textItem(dwr);
    dwr_avas->setAlignment(Qt::AlignLeft);
    dwr_avas->setPos(15, 8);
    dwr_avas->setSize(270, 24);

    dwr_sb = new scrollBar(dwr);
    dwr_sb->setPos(0, 8 + 24);
    dwr_sb->setSize(292, this->height() - 16 - 28);
    dwr_sb->setZValue(0);
    connect(dwr_sb, SIGNAL(valueChanged()), this, SLOT(dwr_sb_valueChanged()));

    dwr_parent = new QGraphicsRectItem(0, -8, 284, this->height(), dwr_sb);
    dwr_parent->setPen(QColor(Qt::transparent));
    dwr_parent->setBrush(Qt::transparent);

    dwr_a = new QPropertyAnimation(dwr, "pos", this);
    dwr_a->setDuration(drawerAnimationDuration);
    dwr_a->setEasingCurve(QEasingCurve::InQuad);
}

void GameWidget::initializeAchievements()
{
    achievementData::known_combinations = &known_combinations;
    achievementData::known_elements = &known_elements;
    achievementData::workspace = &workspace;

    initializeAllAchievements();

    for (int i = 0; i < achievement::allAchievements.count(); i++)
        connect(achievement::allAchievements[i], SIGNAL(achieved()), this, SLOT(achievementAchieved()));
}

void GameWidget::loadGame(const QString &username)
{
    this->username = username;
    known_combinations.clear();
    known_elements.clear();
    workspace.clear();

    configs cf;

    if (cf.load(ResourcesDir.arg(QString("saves/%0.sav").arg(username))))
    {
        for (int i = 0; i < cf.values[0].count(); i++)
        {
            int index = element::allElements.findElementByID(cf.values[0][i]);
            if (index != -1)
                addElementToDrawer(element::allElements[index]);
        }

        for (int i = 0; i < cf.values[1].count(); i++)
            known_combinations << cf.values[1][i].toInt();

        achievement::allAchievements.reset();
        for (int i = 0; i < cf.values[2].count(); i++)
            achievement::allAchievements[i]->setAchieveDate(QDateTime::fromString(cf.values[2][i], "yyyyMMddhhmmss"));
    }
    else
    {
        for (int i = 0; i < 4; i++)
            addElementToDrawer(element::allElements[i]);
    }

    updateDwr_avas();
}

void GameWidget::saveGame()
{
    configs cf;
    cf.keys << "known_elements" << "known_combinations" << "achievements";

    QList<QString> ke;
    for (int i = 0; i < known_elements.length(); i++)
        ke << QString("%0").arg(known_elements[i]->base().id());

    QList<QString> kc;
    for (int i = 0; i < known_combinations.length(); i++)
        kc << QString("%0").arg(QString::number(known_combinations[i]));

    QList<QString> ac;
    for (int i = 0; i < achievement::allAchievements.count(); i++)
        ac << achievement::allAchievements[i]->achieveDate().toString("yyyyMMddhhmmss");

    cf.values << ke << kc << ac;

    QDir d(ResourcesDir);
    if (!d.exists("saves")) d.mkdir("saves");

    cf.save(ResourcesDir.arg(QString("saves/%0.sav").arg(username)));
}

void GameWidget::updateDwr_avas()
{
    dwr_avas->setText(QString("<font color=\"white\">已开放元素：%0/%1</font>").arg(known_elements.count()).arg(element::allElements.count()));
}

void GameWidget::setScrollBars()
{
    ws_gv->horizontalScrollBar()->setMaximum(0);
    ws_gv->verticalScrollBar()->setMaximum(0);

    int dwr_max = ceil(known_elements.length() / 4.0) * 84 - dwr_sb->size().height();
    if (dwr_max < 0) dwr_max = 0;
    dwr_sb->setMaximum(dwr_max / 84.0);
}

void GameWidget::fadeIn_frameChanged(int frame)
{
    elementItem *fadeIn_target = qobject_cast<QTimeLineE*>(sender())->target;
    fadeIn_target->setOpacity(frame / AnimationDurationF);
}

void GameWidget::fadeOut_finished()
{
    elementItem *fadeOut_target = qobject_cast<QTimeLineE*>(sender())->target;
    ws_parent->childItems().removeOne(fadeOut_target);
}

void GameWidget::fadeOut_frameChanged(int frame)
{
    elementItem *fadeOut_target = qobject_cast<QTimeLineE*>(sender())->target;
    fadeOut_target->setOpacity(1 - frame / AnimationDurationF);
}

void GameWidget::elementItem_copyElementItem()
{
    elementItem *s = qobject_cast<elementItem*>(sender());
    addElementToWorkspace(s->base())->setPos(s->pos());
}

void GameWidget::elementItems_posChanged()
{
    elementItem *e = qobject_cast<elementItem*>(sender());

    // 判断是否拖入垃圾箱中
    if (e->collidesWithItem(deleteItem))
    {
        if (deleteGlow->color().alpha() == 0)
        {
            deleteGlowAnimation->stop();
            deleteGlowAnimation->setDirection(QPropertyAnimation::Forward);
            deleteGlowAnimation->start();
        }
    }
    else
    {
        if (deleteGlow->color().alpha() == 255)
        {
            deleteGlowAnimation->stop();
            deleteGlowAnimation->setDirection(QPropertyAnimation::Backward);
            deleteGlowAnimation->start();
        }
    }
}

void GameWidget::elementItems_mousePressed(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        workspace.bringToFront(qobject_cast<elementItem *>(sender()));

        deleteAnimation->setStartValue(QPointF(-48, this->height() / 2.0 - 24));
        deleteAnimation->setEndValue(QPointF(5, this->height() / 2.0 - 24));
        deleteAnimation->setDirection(QPropertyAnimation::Forward);
        deleteAnimation->start();
    }
}

void GameWidget::elementItems_mouseRightButtonPressed(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        elementDetailDialog *ed = new elementDetailDialog(qobject_cast<elementItem *>(sender())->base(), known_combinations);
        showDialog(qobject_cast<dialogBase*>(ed));
    }
}

void GameWidget::elementItems_mouseReleased(QGraphicsSceneMouseEvent *)
{
    // 先隐藏垃圾箱
    if (deleteItem->x() == 5)
    {
        deleteAnimation->setDirection(QPropertyAnimation::Backward);
        deleteAnimation->start();
    }

    elementItem *e = qobject_cast<elementItem *>(sender());

    // 判断是否拖入垃圾箱中
    if (e->collidesWithItem(deleteItem))
    {
        workspace.remove(e);
        fadeOutAndRemove(e);
        return;
    }

    elementItems es = workspace.collidesItems(e, true);
    int result = combine(es.toElements());

    if (!(es.length() == 0 || result == -1))
    {
        QPoint center = es[0]->pos().toPoint();
        for (int i = 1; i < es.length(); i++)
            center = QPoint((center.x() + es[i]->pos().toPoint().x()) / 2, (center.y() + es[i]->pos().toPoint().y()) / 2);

        for (int i = 0; i < es.length(); i++)
        {
            workspace.remove(es[i]);
            fadeOutAndRemove(es[i]);
        }

        combination *c = &combination::allCombinations[result];
        bool save = false;

        for (int i = 0; i < c->product.length(); i++)
        {
            if (known_elements.findItemByID(c->product[i].id()) == -1)
            {
                addElementToDrawer(c->product[i]);
                save = true;
            }

            addElementToWorkspace(c->product[i])->setPos(center);
        }

        if (known_combinations.indexOf(result) == -1)
        {
            known_combinations << result;
            save = true;
        }

        if (save)
        {
            achievement::allAchievements.checkForAll();
            saveGame();
        }
    }

    setScrollBars();
}

void GameWidget::ws_parent_mouseDoubleClicked(QGraphicsSceneMouseEvent *e)
{
    // 扩展：鼠标左键，右键，中键双击出不同元素
    for (int i = 0; i < 4; i++)
        addElementToWorkspace(element::allElements[i])->setPos(e->pos().x() + (i - 2) * 64, e->pos().y() - 84 / 2);
}

void GameWidget::dwr_hoverEnter(QGraphicsSceneHoverEvent *)
{
    dwr_a->stop();
    dwr_a->setStartValue(dwr->pos());
    dwr_a->setEndValue(QPoint(this->width() - 300, 0));
    dwr_a->start();
}

void GameWidget::dwr_hoverLeave(QGraphicsSceneHoverEvent *)
{
    dwr_a->stop();
    dwr_a->setStartValue(dwr->pos());
    dwr_a->setEndValue(QPoint(this->width() - 5, 0));
    dwr_a->start();
}

void GameWidget::dwr_sb_valueChanged()
{
    dwr_parent->setPos(0, -dwr_sb->value() * 84);
    setScrollBars();
}

void GameWidget::dialog_resized(QResizeEvent *)
{
    dialog->setPos(this->width() / 2 - (int)dialog->size().width() / 2, this->height() / 2 - (int)dialog->size().height() / 2);
}

void GameWidget::shield_mouseReleased()
{
    // closeDialog()
    dialog_a->setDirection(QPropertyAnimation::Backward);
    dialog_a->start();
    ws_blur_a->setDirection(QPropertyAnimation::Backward);
    ws_blur_a->start();
}

void GameWidget::dialog_a_finished()
{
    if (dialog_a->direction() == QPropertyAnimation::Forward) return;

    delete dialog; dialog = 0;
    ws_scene->removeItem(shield);
}

void GameWidget::resizeEvent(QResizeEvent *e)
{
    ws_parent->setSize(this->geometry().width(), this->geometry().height());
    ws_pgv->resize(this->geometry().size());
    ws_gv->resize(this->geometry().width() + 2, this->geometry().height() + 2);

    dwr_parent->setRect(0, -8, 284, this->height());
    if (e->oldSize().width() != -1)
        dwr->setPos(dwr->x() + (this->width() - e->oldSize().width()), 0);
    dwr->updateBuffer(QSizeF(300, this->height()));
    dwr_sb->setSize(292, this->height() - 16 - 28);

    if (dialog) dialog_resized(NULL);
    shield->setSize(this->size());

    setScrollBars();
}
