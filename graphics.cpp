#include "graphics.h"
#include <algorithm>
#include <cmath>
using namespace std;

void graphicsItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    paintEvent(painter);
    emit userPaint(painter);
}

void graphicsItemBase::setSize(const QSizeF &value)
{
    QSizeF *oldValue = &_size;
    _size = value;

    resizeEvent(new QResizeEvent(_size.toSize(), oldValue->toSize()));
    this->update();
}

void graphicsViewBase::mousePressEvent(QMouseEvent *event)
{
    emit mousePress(event);
    QGraphicsView::mousePressEvent(event);
}

drawerGraphicsItem::drawerGraphicsItem(QGraphicsItem *parent)  : graphicsItemBase(parent)
{
    scaledImageBuffer = QImage(":/data/linen.png").scaled(drawerSize - 5, drawerSize - 5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setAcceptHoverEvents(true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setOffset(-3, 0);
    this->setGraphicsEffect(shadow);
}

void drawerGraphicsItem::paintEvent(QPainter *p)
{
    p->drawImage(0, 0, buffer);
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

    painter->setPen(QPen(Qt::transparent));
    painter->translate(5, 0);
    painter->drawImage(0, 0, backgroundBuffer);

    QLinearGradient bg(0, 0, 0, size.height());
    bg.setColorAt(0, QColor::fromRgb(0, 0, 0, 25));
    bg.setColorAt(1, QColor::fromRgb(0, 0, 0, 150));

    painter->setBrush(bg);
    painter->drawRect(0, 0, size.width(), size.height());

    // 边缘处会出现杂色，这样可以清除杂色
    painter->translate(-5, 0);
    painter->setCompositionMode(QPainter::CompositionMode_Clear);
    painter->drawRect(0, 0, 5, size.height());

    painter->end(); delete painter;
    _size = size;
}

dialogBase::dialogBase(QGraphicsItem *parent) : graphicsItemBase(parent)
{
    _shadow = new QGraphicsDropShadowEffect(this);
    _shadow->setBlurRadius(25);
    _shadow->setColor(QColor(0, 0, 0, 160));
    _shadow->setOffset(0, 5);
    this->setGraphicsEffect(_shadow);

    this->setAcceptHoverEvents(true);
}

void dialogBase::paintEvent(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->translate(0.5, 0.5);

    p->setPen(QColor(Qt::gray));
    p->setBrush(QColor(255, 255, 255, 255));
    p->drawRoundedRect(this->boundingRect(), 5, 5);
}
