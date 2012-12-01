#include "MainWindow.h"
#include "helper.h"
#include <cmath>
#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(550, 400);
    this->resize(550, 400);
    this->setWindowTitle(tr("Alchemy"));

    initializeWorkspace();
    initializeDrawer();

    // 默认调试用户
    loadGame("tester");

    for (int i = 0; i < 4; i++)
        addElementToWorkspace(element::allElements[i])->setPos(this->width() / 2 + (i - 2) * 64, this->height() / 2 - 84 / 2);
}

elementItem* MainWindow::addElementToWorkspace(const element &e)
{
    elementItem *ei = new elementItem(e);
    ei->setZValue(workspace.length());
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mousePressed(QGraphicsSceneMouseEvent*)));
    connect(ei, SIGNAL(mouseReleased(QGraphicsSceneMouseEvent*)), this, SLOT(elementItems_mouseReleased(QGraphicsSceneMouseEvent*)));

    workspace << ei;
    fadeInAndAdd(ei);

    return ei;
}

void MainWindow::addElementToWorkspace(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        addElementToWorkspace(qobject_cast<elementItem*>(sender())->base())->setPos(this->width() / 2 - 64 / 2, this->height() / 2 - 84 / 2);
        showOrHideDrawer();
    }
}

elementItem* MainWindow::addElementToDrawer(const element &e)
{
    elementItem *ei = new elementItem(e);
    ei->setDrawerStyle(true);
    ei->setDrag(false);
    ei->setParentItem(dwr_parent);
    connect(ei, SIGNAL(mousePressed(QGraphicsSceneMouseEvent*)), this, SLOT(addElementToWorkspace(QGraphicsSceneMouseEvent*)));

    if (known_elements.length() == 0)
    {
        ei->setPos(22, 10);
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
                    known_elements[j]->setPos(j % 4 * 64 + 22, j / 4 * 84 + 10);
                }

                break;
            }
        }

        if (i == known_elements.length())
        {
            ei->setPos(known_elements.length() % 4 * 64 + 22, known_elements.length() / 4 * 84 + 10);
            known_elements << ei;
        }
    }

    return ei;
}

void MainWindow::fadeInAndAdd(elementItem *item)
{
    item->setOpacity(0);
    item->setParentItem(ws_parent);

    QTimeLineE *tl = new QTimeLineE(250, this);
    tl->target = item;
    tl->setFrameRange(0, 200);
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(fadeIn_frameChanged(int)));

    tl->start();
}

void MainWindow::fadeOutAndRemove(elementItem *item)
{
    QTimeLineE *tl = new QTimeLineE(250);
    tl->target = item;
    tl->setFrameRange(0, 200);
    connect(tl, SIGNAL(finished()), this, SLOT(fadeOut_finished()));
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(fadeOut_frameChanged(int)));

    tl->start();
}

void MainWindow::initializeWorkspace()
{
    ws_scene = new QGraphicsScene();
    ws_parent = ws_scene->addRect(0, 0, this->width(), this->height(), QPen(Qt::transparent));

    ws_pgv = new QWidget(this);
    ws_pgv->setGeometry(0, 0, this->width(), this->height());

    ws_gv = new graphicsViewBase(ws_scene, ws_pgv);
    ws_gv->setMouseTracking(true);
    ws_gv->setGeometry(-1, -1, this->width() + 2, this->height() + 2);
    ws_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ws_gv->setRenderHints(QPainter::Antialiasing);
    ws_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ws_gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    connect(ws_gv, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(ws_gv_mousePressed(QMouseEvent*)));

    ws_a = new QPropertyAnimation(ws_pgv, "pos", this);
    ws_a->setDuration(drawerAnimationDuration);
    ws_a->setEasingCurve(QEasingCurve::InQuad);

    QPushButton *add = new QPushButton(ws_pgv);
    add->setText(tr("add"));
    add->setGeometry(this->width() - 15 - 75, this->height() - 15 - 25, 75, 25);
    connect(add, SIGNAL(clicked()), this, SLOT(showOrHideDrawer()));
}

void MainWindow::initializeDrawer()
{
    dwr_scene = new drawerGraphicsScene();
    dwr_parent = dwr_scene->addRect(0, 0, 300, this->height(), QPen(Qt::transparent), QBrush(Qt::transparent));

    dwr_pgv = new QWidget(this);
    dwr_pgv->setGeometry(this->width() + 1, 0, 300, this->height());

    dwr_gv = new graphicsViewBase(dwr_scene, dwr_pgv);
    dwr_gv->setMouseTracking(true);
    dwr_gv->setGeometry(-1, -1, 302, this->height() + 2);
    dwr_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dwr_gv->setRenderHints(QPainter::Antialiasing);
    dwr_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    dwr_gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    dwr_a = new QPropertyAnimation(dwr_pgv, "pos", this);
    dwr_a->setDuration(drawerAnimationDuration);
    dwr_a->setEasingCurve(QEasingCurve::InQuad);

    drawerOpened = false;
}

void MainWindow::loadGame(const QString &username)
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

void MainWindow::saveGame()
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

void MainWindow::setScrollBars()
{
    ws_gv->horizontalScrollBar()->setMaximum(0);
    ws_gv->verticalScrollBar()->setMaximum(0);

    int dwr_max = ceil(known_elements.length() / 4.0) * 84 + 20 - this->height();
    if (dwr_max < 0) dwr_max = 0;
    dwr_gv->verticalScrollBar()->setMaximum(dwr_max);
}

void MainWindow::fadeIn_frameChanged(int frame)
{
    elementItem *fadeIn_target = qobject_cast<QTimeLineE *>(sender())->target;
    fadeIn_target->setOpacity(frame / 200.0);
}

void MainWindow::fadeOut_finished()
{
    elementItem *fadeOut_target = qobject_cast<QTimeLineE *>(sender())->target;
    ws_parent->childItems().removeOne(fadeOut_target);
}

void MainWindow::fadeOut_frameChanged(int frame)
{
    elementItem *fadeOut_target = qobject_cast<QTimeLineE *>(sender())->target;
    fadeOut_target->setOpacity(1 - frame / 200.0);
}

void MainWindow::elementItems_mousePressed(QGraphicsSceneMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        workspace.bringToFront(qobject_cast<elementItem *>(sender()));
    else if (e->button() == Qt::RightButton)
    {
        elementItem *ei = qobject_cast<elementItem *>(sender());

        for (int i = 0; i < known_combinations.count(); i++)
        {
            combination *c = &combination::allCombinations[known_combinations[i]];

            if (c->source.findElementByID(ei->base().id()) != -1 ||
                c->product.findElementByID(ei->base().id()) != -1)
            {
                QString out;
                for (int j = 0; j < c->source.count(); j++)
                {
                    out.append(c->source[j].name());
                    if (j != c->source.count() - 1) out.append(" + ");
                    else out.append(" = ");
                }

                for (int j = 0; j < c->product.count(); j++)
                {
                    out.append(c->product[j].name());
                    if (j != c->product.count() - 1) out.append(" + ");
                }

                qDebug() << out;
            }
        }
    }
}

void MainWindow::elementItems_mouseReleased(QGraphicsSceneMouseEvent *)
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

void MainWindow::ws_gv_mousePressed(QMouseEvent *)
{
    if (drawerOpened)
        showOrHideDrawer();
}

void MainWindow::showOrHideDrawer()
{
    ws_a->stop(); ws_a->setStartValue(ws_pgv->pos());
    dwr_a->stop(); dwr_a->setStartValue(dwr_pgv->pos());

    drawerOpened = !drawerOpened;

    if (drawerOpened)
    {
        ws_a->setEndValue(QPoint(-300, 0));
        dwr_a->setEndValue(QPoint(ws_pgv->width() - 300 + 1, 0));
    }
    else
    {
        ws_a->setEndValue(QPoint(0, 0));
        dwr_a->setEndValue(QPoint(ws_pgv->width() + 1, 0));
    }

    ws_a->start(); dwr_a->start();
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    ws_parent->setRect(0, 0, this->geometry().width(), this->geometry().height());
    ws_pgv->resize(this->geometry().size());
    ws_gv->resize(this->geometry().width() + 2, this->geometry().height() + 2);

    dwr_parent->setRect(0, 0, 300, this->height());
    dwr_scene->updateBuffer(QSizeF(300, this->height()));
    dwr_pgv->setGeometry(dwr_pgv->x() + (this->width() - e->oldSize().width()), 0, 300, this->height());
    dwr_gv->resize(302, this->geometry().height() + 2);

    setScrollBars();
}

void MainWindow::paintEvent(QPaintEvent *)
{
//    QPainter *painter = new QPainter(this);
//    painter->setBrush(Qt::white);
//    painter->setPen(Qt::transparent);
//    painter->drawRect(0, 0, this->width(), this->height());
//    painter->end(); delete painter;
}
