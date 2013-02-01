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

drawerGraphicsItem::drawerGraphicsItem(QGraphicsItem *parent)  : graphicsItemBase(parent)
{
    scaledImageBuffer = QImage(":/res/linen.png").scaled(drawerSize - 5, drawerSize - 5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setAcceptHoverEvents(true);
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

    int ch = ceil(size.height() / (drawerSize - 5));
    if (ch * drawerSize != backgroundBuffer.height())
    {
        backgroundBuffer = QImage(drawerSize, ch * drawerSize, QImage::Format_ARGB32);
        QPainter *bpainter = new QPainter(&backgroundBuffer);

        for (int i = 0; i < ch; i++)
            bpainter->drawImage(0, i * (drawerSize - 5), scaledImageBuffer);

        bpainter->end(); delete bpainter;
    }

    QLinearGradient a(0, 0, 5, 0);
    a.setColorAt(0, QColor::fromRgb(0, 0, 0, 0));
    a.setColorAt(1, QColor::fromRgb(0, 0, 0, 96));
    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(a);
    painter->drawRect(0, 0, 5, size.height());

    painter->translate(5, 0);
    painter->drawImage(0, 0, backgroundBuffer);

    QLinearGradient bg(0, 0, 0, size.height());
    bg.setColorAt(0, QColor::fromRgb(0, 0, 0, 25));
    bg.setColorAt(1, QColor::fromRgb(0, 0, 0, 150));

    painter->setBrush(bg);
    painter->drawRect(0, 0, size.width(), size.height());

    painter->end(); delete painter;
    _size = size;
}

