#include "graphics.h"
#include <algorithm>
#include <cmath>
using namespace std;

void graphicsItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    paintEvent(painter);
    emit userPaint(painter);
}

void graphicsViewBase::mousePressEvent(QMouseEvent *event)
{
    emit mousePress(event);
    QGraphicsView::mousePressEvent(event);
}

drawerGraphicsScene::drawerGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    scaledImageBuffer = QImage(":/res/linen.png").scaled(drawerSize, drawerSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void drawerGraphicsScene::updateBuffer(const QSizeF &size)
{
    buffer = QImage(size.toSize(), QImage::Format_ARGB32);
    QPainter *painter = new QPainter(&buffer);

    int ch = ceil(size.height() / drawerSize);
    if (ch * drawerSize != backgroundBuffer.height())
    {
        backgroundBuffer = QImage(300, ch * drawerSize, QImage::Format_ARGB32);
        QPainter *bpainter = new QPainter(&backgroundBuffer);

        for (int i = 0; i < ch; i++)
            bpainter->drawImage(0, i * drawerSize, scaledImageBuffer);

        bpainter->end(); delete bpainter;
    }

    painter->drawImage(0, 0, backgroundBuffer);

    QLinearGradient bg(0, 0, 0, size.height());
    bg.setColorAt(0, QColor::fromRgb(0, 0, 0, 25));
    bg.setColorAt(1, QColor::fromRgb(0, 0, 0, 150));

    painter->setBrush(bg);
    painter->setPen(QPen(Qt::transparent));
    painter->drawRect(0, 0, size.width(), size.height());

    QLinearGradient border(0, 0, 10, 0);
    border.setColorAt(0, QColor::fromRgb(0, 0, 0, 64));
    border.setColorAt(1, QColor::fromRgb(0, 0, 0, 0));

    painter->setBrush(border);
    painter->drawRect(0, 0, 10, size.height());

    QLinearGradient border1(0, 0, 0, 10);
    border1.setColorAt(0, QColor::fromRgb(0, 0, 0, 64));
    border1.setColorAt(1, QColor::fromRgb(0, 0, 0, 0));

    painter->setBrush(border1);
    painter->drawRect(0, 0, size.width(), 10);

    painter->end(); delete painter;
}

void drawerGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->translate(rect.x(), rect.y());
    painter->drawImage(0, 0, buffer);
}

void drawerGraphicsItem::paintEvent(QPainter *p)
{
    p->drawImage(0, 0, buffer);
}

QRectF drawerGraphicsItem::boundingRect() const
{
    return QRectF(QPointF(0, 0), _size);
}

void drawerGraphicsItem::updateBuffer(const QSizeF &size)
{
    buffer = QImage(size.toSize(), QImage::Format_ARGB32);
    QPainter *painter = new QPainter(&buffer);

    int ch = ceil(size.height() / drawerSize);
    if (ch * drawerSize != backgroundBuffer.height())
    {
        backgroundBuffer = QImage(300, ch * drawerSize, QImage::Format_ARGB32);
        QPainter *bpainter = new QPainter(&backgroundBuffer);

        for (int i = 0; i < ch; i++)
            bpainter->drawImage(0, i * drawerSize, scaledImageBuffer);

        bpainter->end(); delete bpainter;
    }

    painter->drawImage(0, 0, backgroundBuffer);

    QLinearGradient bg(0, 0, 0, size.height());
    bg.setColorAt(0, QColor::fromRgb(0, 0, 0, 25));
    bg.setColorAt(1, QColor::fromRgb(0, 0, 0, 150));

    painter->setBrush(bg);
    painter->setPen(QPen(Qt::transparent));
    painter->drawRect(0, 0, size.width(), size.height());

    QLinearGradient border(0, 0, 10, 0);
    border.setColorAt(0, QColor::fromRgb(0, 0, 0, 64));
    border.setColorAt(1, QColor::fromRgb(0, 0, 0, 0));

    painter->setBrush(border);
    painter->drawRect(0, 0, 10, size.height());

    QLinearGradient border1(0, 0, 0, 10);
    border1.setColorAt(0, QColor::fromRgb(0, 0, 0, 64));
    border1.setColorAt(1, QColor::fromRgb(0, 0, 0, 0));

    painter->setBrush(border1);
    painter->drawRect(0, 0, size.width(), 10);

    painter->end(); delete painter;
    _size = size;
}

