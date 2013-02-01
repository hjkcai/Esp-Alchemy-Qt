#include "scrollbar.h"

ScrollBar::ScrollBar(QGraphicsItem *parent) :
    graphicsItemBase(parent)
{
    _fade = new QPropertyAnimation(this, "drawOpacity");
    _fade->setStartValue(0.2);
    _fade->setEndValue(0.4);
    _fade->setDuration(100);

    _scroll = new QPropertyAnimation(this, "value");
    _scroll->setDuration(200);
    _scroll->setEasingCurve(QEasingCurve::OutQuad);

    _max = 0; _min = 0; _value = 0; deltaHeight = 0; _drawOpacity = 1;
    handlerBuf = QRectF(0, 0, _size.width(), 0);

    _pressed = false;
    _Yoffset = 0;

    this->setAcceptHoverEvents(true);
    this->setDrawOpacity(0.2);
}

QRectF ScrollBar::boundingRect() const
{
    return QRectF(0, 0, _size.width(), _size.height());
}

void ScrollBar::setMaximum(const double &value)
{
    if (value < _min) _max = _min;
    else _max = value;

    valueRange = _max - _min;
    _update();
}

void ScrollBar::setMinimum(const double &value)
{
    if (value > _max) _min = _max;
    else _min = value;

    valueRange = _max - _min;
    _update();
}

void ScrollBar::setValue(const double &value)
{
    if (value > _max) _value = _max;
    else if (value < _min) _value = _min;
    else _value = value;

    _update();
    emit valueChanged();
}

void ScrollBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    if (_drawOpacity != 0.2)
    {
        _fade->stop();
        _fade->setDirection(QPropertyAnimation::Backward);
        _fade->start();
    }
}

void ScrollBar::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
//    qDebug() << handlerBuf.x() << handlerBuf.y() << handlerBuf.right() << handlerBuf.bottom();
//    qDebug() << e->pos().x() << e->pos().y();

    if (_drawOpacity == 0.2 && intersects(e->pos()))
    {
        _fade->stop();
        _fade->setDirection(QPropertyAnimation::Forward);
        _fade->start();
    }
    else if (_drawOpacity != 0.2 && !intersects(e->pos()) && _fade->direction() == QPropertyAnimation::Forward)
    {
        _fade->stop();
        _fade->setDirection(QPropertyAnimation::Backward);
        _fade->start();
    }
}

void ScrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (_pressed)
        setValue(valueRange / deltaHeight * (e->pos().y() - _Yoffset));
}

void ScrollBar::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (intersects(e->pos()))
    {
        _pressed = true;
        _Yoffset = e->pos().y() - handlerBuf.y();
    }
    else if (e->pos().x() >= _size.width() - 8)
        setValue(valueRange / deltaHeight * (e->pos().y() - handlerBuf.height() / 2));
}

void ScrollBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    _pressed = false;
}

void ScrollBar::paintEvent(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->translate(0.5, 0.5);
    p->setOpacity(_drawOpacity);

    p->setPen(QColor::fromRgb(255, 255, 255, 128));
    p->setBrush(Qt::black);
    p->drawRoundedRect(handlerBuf, 4, 4);
}

void ScrollBar::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    _scroll->stop();
    _scroll->setStartValue(_value);

    double end = _scroll->endValue().toDouble();
    if (event->delta() < 0) // up
        end++;
    else end--;

    if (end > _max) end = _max;
    else if (end < _min) end = _min;
    _scroll->setEndValue(end);

    if (_scroll->startValue() != _scroll->endValue())
        _scroll->start();
}

void ScrollBar::updateHandler()
{
    double h = _size.height() - _size.height() / 5 * sqrt(valueRange);
    if (h < 20) h = 20;

    deltaHeight = _size.height() - h;

    handlerBuf = QRectF(_size.width() - 8,
                        valueRange == 0 ? 0 : deltaHeight / valueRange * _value,
                        8,
                        h);
}
