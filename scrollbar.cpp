#include "scrollbar.h"

ScrollBar::ScrollBar(QGraphicsItem *parent) :
    graphicsItemBase(parent)
{
    _fade = new QPropertyAnimation(this, "opacity");
    _fade->setStartValue(0.2);
    _fade->setEndValue(0.4);
    _fade->setDuration(100);

    _scroll = new QPropertyAnimation(this, "value");
    _scroll->setDuration(200);
    _scroll->setEasingCurve(QEasingCurve::OutQuad);

    _max = 0; _min = 0; _value = 0; deltaHeight = 0;
    handlerBuf = QRectF(0, 0, _size.width(), 0);

    _pressed = false;
    _Yoffset = 0;

    this->setAcceptHoverEvents(true);
    this->setOpacity(0.2);
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

void ScrollBar::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    _fade->stop();
    _fade->setDirection(QPropertyAnimation::Forward);
    _fade->start();
}

void ScrollBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    _fade->stop();
    _fade->setDirection(QPropertyAnimation::Backward);
    _fade->start();
}

void ScrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (_pressed)
    {
        setValue(valueRange / deltaHeight * (e->pos().y() - _Yoffset/* - handlerBuf.height() / 2*/));
    }
}

void ScrollBar::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->pos().y() >= handlerBuf.y() && e->pos().y() <= handlerBuf.bottom())
    {
        _pressed = true;
        _Yoffset = e->pos().y() - handlerBuf.y();
    }
    else
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

    //p->drawRect(this->boundingRect());

    p->setPen(QColor::fromRgb(255, 255, 255, 128));
    p->setBrush(Qt::black);
    p->drawRoundedRect(handlerBuf, 4, 4);
}

void ScrollBar::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    _scroll->stop();
    _scroll->setStartValue(_value);

    if (event->delta() < 0) // up
        _scroll->setEndValue(_value + 3 > _max ? _max : _value + 3);
    else _scroll->setEndValue(_value - 3 < _min ? _min : _value - 3);

    if (_scroll->startValue() != _scroll->endValue())
        _scroll->start();
}

void ScrollBar::updateHandler()
{
    double h = _size.height() - _size.height() / 5 * sqrt(valueRange);
    if (h < 20) h = 20;

    deltaHeight = _size.height() - h;

    handlerBuf = QRectF(0,
                        valueRange == 0 ? 0 : deltaHeight / valueRange * _value,
                        _size.width(),
                        h);
}
