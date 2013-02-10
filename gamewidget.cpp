#include "gamewidget.h"
#include "elementdetaildialog.h"
#include "helper.h"
#include <cmath>
#include <string>
using namespace std;

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(550, 400);
    this->resize(550, 400);
    this->setWindowTitle(tr("Alchemy"));

    initializeDialog();
    initializeWorkspace();
    initializeDrawer();

    // 默认调试用户
    loadGame("tester");

    for (int i = 0; i < 4; i++)
        addElementToWorkspace(element::allElements[i])->setPos(this->width() / 2 + (i - 2) * 64, this->height() / 2 - 84 / 2);
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
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mousePressed(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mouseRightButtonPressed(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(mouseReleased(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mouseReleased(QGraphicsSceneMouseEvent*)));

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
        //showOrHideDrawer();
    }
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
        ei->setPos(18, 5);
        known_elements << ei;
    }
    else
    {
        int i;
        for (i = 0; i < known_elements.length(); i++)
        {
            if (e.id().compare(known_elements[i]->base().id()) < 0)
            {
                known_elements.insert(i, ei);
                for (int j = i; j < known_elements.length(); j++)
                {
                    known_elements[j]->setPos(j % 4 * 64 + 18, j / 4 * 84 + 5);
                }

                break;
            }
        }

        if (i == known_elements.length())
        {
            ei->setPos(known_elements.length() % 4 * 64 + 18, known_elements.length() / 4 * 84 + 5);
            known_elements << ei;
        }
    }

    return ei;
}

void GameWidget::fadeInAndAdd(elementItem *item)
{
    item->setOpacity(0);
    item->setParentItem(ws_parent);

    QTimeLineE *tl = new QTimeLineE(250, this);
    tl->target = item;
    tl->setFrameRange(0, 200);
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(fadeIn_frameChanged(int)));

    tl->start();
}

void GameWidget::fadeOutAndRemove(elementItem *item)
{
    QTimeLineE *tl = new QTimeLineE(250);
    tl->target = item;
    tl->setFrameRange(0, 200);
    connect(tl, SIGNAL(finished()), this, SLOT(fadeOut_finished()));
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(fadeOut_frameChanged(int)));

    tl->start();
}

void GameWidget::initializeDialog()
{
    dialog = NULL;

    shield = new mouseShield();
    shield->setSize(550, 400);
    connect(shield, SIGNAL(mouseReleased()), this, SLOT(shield_mouseReleased()));

    dialog_a = new QPropertyAnimation(shield, "opacity", this);
    dialog_a->setStartValue(0);
    dialog_a->setEndValue(1);
    dialog_a->setDuration(200);
    connect(dialog_a, SIGNAL(finished()), this, SLOT(dialog_a_finished()));
}

void GameWidget::initializeWorkspace()
{
    ws_scene = new QGraphicsScene();
    ws_parent = ws_scene->addRect(0, 0, this->width(), this->height(), QPen(Qt::transparent), QBrush(Qt::white));

    ws_pgv = new QWidget(this);
    ws_pgv->setGeometry(0, 0, this->width(), this->height());

    ws_gv = new graphicsViewBase(ws_scene, ws_pgv);
    ws_gv->setMouseTracking(true);
    ws_gv->setGeometry(-1, -1, this->width() + 2, this->height() + 2);
    ws_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ws_gv->setRenderHints(QPainter::Antialiasing);
    ws_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ws_gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    ws_blur = new QGraphicsBlurEffect(this);
    ws_blur->setBlurRadius(0);
    ws_parent->setGraphicsEffect(ws_blur);

    ws_blur_a = new QPropertyAnimation(ws_blur, "blurRadius", this);
    ws_blur_a->setStartValue(0);
    ws_blur_a->setEndValue(5);
    ws_blur_a->setDuration(200);
}

void GameWidget::initializeDrawer()
{
    dwr = new drawerGraphicsItem();
    dwr->setPos(this->width() - 5, 0);
    dwr->updateBuffer(QSizeF(300, this->height()));
    connect(dwr, SIGNAL(hoverEnter(QGraphicsSceneHoverEvent*)), this, SLOT(dwr_hoverEnter(QGraphicsSceneHoverEvent*)));
    connect(dwr, SIGNAL(hoverLeave(QGraphicsSceneHoverEvent*)), this, SLOT(dwr_hoverLeave(QGraphicsSceneHoverEvent*)));
    ws_scene->addItem(dwr);

    dwr_sb = new scrollBar(dwr);
    dwr_sb->setPos(0, 8);
    dwr_sb->setSize(292, this->height() - 16);
    dwr_sb->setZValue(0);
    connect(dwr_sb, SIGNAL(valueChanged()), this, SLOT(dwr_sb_valueChanged()));

    dwr_parent = new QGraphicsRectItem(0, -8, 284, this->height(), dwr_sb);
    dwr_parent->setPen(QColor(Qt::transparent));
    dwr_parent->setBrush(Qt::transparent);

    dwr_a = new QPropertyAnimation(dwr, "pos", this);
    dwr_a->setDuration(drawerAnimationDuration);
    dwr_a->setEasingCurve(QEasingCurve::InQuad);
}

void GameWidget::loadGame(const QString &username)
{
    this->username = username;
    known_combinations.clear();
    known_elements.clear();
    workspace.clear();
//    ws_parent->children().clear()
//    dwr_parent->children().clear();

    QFile *saves = new QFile(QString("%0/saves/%1.sav").arg(QApplication::applicationDirPath()).arg(username));
    if (saves->exists())
    {
        saves->open(QFile::ReadOnly);
        QString source = QString::fromStdString(des.Des_DecryptText(QString(saves->readAll()).toStdString(), username.toStdString())).trimmed();
        QStringList ke = subString(source, QString::null, "\n").trimmed().split('|'); //known_elements
        QStringList kc = subString(source, "\n", QString::null).trimmed().split('|'); //known_combinations

        for (int i = 0; i < ke.count() - 1; i++)
        {
            int index = element::allElements.findElementByID(ke[i]);
            if (index != -1)
                addElementToDrawer(element::allElements[index]);
        }

        for (int i = 0; i < kc.count() - 1; i++)
            known_combinations << kc[i].toInt();
    }
    else
    {
        for (int i = 0; i < 4; i++)
            addElementToDrawer(element::allElements[i]);
    }

    delete saves;
}

void GameWidget::saveGame()
{
    QString data;
    for (int i = 0; i < known_elements.length(); i++)
        data.append(QString("%0|").arg(known_elements[i]->base().id()));

    data.append('\n');

    for (int i = 0; i < known_combinations.length(); i++)
        data.append(QString("%0|").arg(QString::number(known_combinations[i])));

    data = QString::fromStdString(des.Des_EncryptText(data.toStdString(), username.toStdString()));

    QFile *saves = new QFile(QString("%0/saves/%1.sav").arg(QApplication::applicationDirPath()).arg(username));

    QDir d(QApplication::applicationDirPath());
    if (!d.exists("saves")) d.mkdir("saves");

    saves->open(QFile::WriteOnly);
    saves->write(data.toAscii());
    saves->waitForBytesWritten(3000);
    saves->flush();
    saves->close();
    delete saves;
}

void GameWidget::setScrollBars()
{
    ws_gv->horizontalScrollBar()->setMaximum(0);
    ws_gv->verticalScrollBar()->setMaximum(0);

    int dwr_max = ceil(known_elements.length() / 4.0) * 84 + 28 - this->height();
    if (dwr_max < 0) dwr_max = 0;
    dwr_sb->setMaximum(dwr_max / 84.0);
}

void GameWidget::fadeIn_frameChanged(int frame)
{
    elementItem *fadeIn_target = qobject_cast<QTimeLineE *>(sender())->target;
    fadeIn_target->setOpacity(frame / 200.0);
}

void GameWidget::fadeOut_finished()
{
    elementItem *fadeOut_target = qobject_cast<QTimeLineE *>(sender())->target;
    ws_parent->childItems().removeOne(fadeOut_target);
}

void GameWidget::fadeOut_frameChanged(int frame)
{
    elementItem *fadeOut_target = qobject_cast<QTimeLineE *>(sender())->target;
    fadeOut_target->setOpacity(1 - frame / 200.0);
}

void GameWidget::elementItems_mousePressed(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        workspace.bringToFront(qobject_cast<elementItem *>(sender()));
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
    elementItems es = workspace.collidesItems(qobject_cast<elementItem *>(sender()), true);
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

        if (save) saveGame();
    }

    setScrollBars();
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
    ws_parent->setRect(0, 0, this->geometry().width(), this->geometry().height());
    ws_pgv->resize(this->geometry().size());
    ws_gv->resize(this->geometry().width() + 2, this->geometry().height() + 2);

    dwr_parent->setRect(0, -8, 284, this->height());
    if (e->oldSize().width() != -1)
        dwr->setPos(dwr->x() + (this->width() - e->oldSize().width()), 0);
    dwr->updateBuffer(QSizeF(300, this->height()));
    dwr_sb->setSize(292, this->height() - 16);

    if (dialog) dialog_resized(NULL);
    shield->setSize(this->size());

    setScrollBars();
}
