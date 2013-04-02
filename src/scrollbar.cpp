#include "scrollbar.h"

scrollBar::scrollBar(QGraphicsItem *parent) :
    graphicsItemBase(parent)
{
    _fade = new QPropertyAnimation(this, "drawOpacity");
    _fade->setStartValue(0.2);
    _fade->setEndValue(0.4);
    _fade->setDuration(100);

    _scroll = new QPropertyAnimation(this, "value");
    _scroll->setDuration(200);
    _scroll->setEasingCurve(QEasingCurve::OutQuad);
    connect(_scroll, SIGNAL(finished()), this, SLOT(_scroll_finished()));

    _max = 0; _min = 0; valueRange = 0; _value = 0; deltaHeight = 0; _drawOpacity = 1; _alwaysShow = true;
    handlerBuf = QRectF(0, 0, _size.width(), 0);

    _pressed = false;
    _Yoffset = 0;

    this->setAcceptHoverEvents(true);
    this->setDrawOpacity(0.2);
}

void scrollBar::setMaximum(const double &value)
{
    if (value < _min) _max = _min;
    else _max = value;

    valueRange = _max - _min;
    _update();
}

void scrollBar::setMinimum(const double &value)
{
    if (value > _max) _min = _max;
    else _min = value;

    valueRange = _max - _min;
    _update();
}

void scrollBar::setValue(const double &value)
{
    bool changed = value != _value;

    if (value > _max) _value = _max;
    else if (value < _min) _value = _min;
    else _value = value;

    _update();
    if (changed) emit valueChanged();
}

void scrollBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    if (_drawOpacity != 0.2)
    {
        _fade->stop();
        _fade->setDirection(QPropertyAnimation::Backward);
        _fade->start();
    }
    else graphicsItemBase::hoverLeaveEvent(e);
}

void scrollBar::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
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
    else graphicsItemBase::hoverMoveEvent(e);
}

void scrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (_pressed)
        setValue(deltaHeight != 0 ? valueRange / deltaHeight * (e->pos().y() - _Yoffset) : 0);
    else graphicsItemBase::mouseMoveEvent(e);
}

void scrollBar::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if (intersects(e->pos()))
    {
        _pressed = true;
        _Yoffset = e->pos().y() - handlerBuf.y();
    }
    else if (e->pos().x() >= _size.width() - 8)
        setValue(valueRange / deltaHeight * (e->pos().y() - handlerBuf.height() / 2));
    else graphicsItemBase::mousePressEvent(e);
}

void scrollBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    _pressed = false;
    if (e->pos().x() < _size.width() - 8)
        graphicsItemBase::mouseReleaseEvent(e);
}

void scrollBar::paintEvent(QPainter *p)
{
    if (valueRange == 0 && !_alwaysShow) return;

    p->setRenderHint(QPainter::Antialiasing);
    p->translate(0.5, 0.5);
    p->setOpacity(_drawOpacity);

    p->setPen(QColor::fromRgb(255, 255, 255, 128));
    p->setBrush(Qt::black);
    p->drawRoundedRect(handlerBuf, 4, 4);
}

void scrollBar::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // 防止过快移动
    // 需要更改为TimeLine型从而更好地控制动画
    int cur = _scroll->currentTime() + (200 - _scroll->duration());
    if (cur > 150) cur = 0;

    _scroll->stop();
    _scroll->setDuration(200 - cur);
    _scroll->setStartValue(_value);

    double end = _scroll->endValue().toDouble();
    if (event->delta() < 0) // up
        end++;
    else end--;

    if (end > _max) end = _max;
    else if (end < _min) end = _min;
    _scroll->setEndValue(end);

    _scroll->start();
}

void scrollBar::_scroll_finished()
{
    _scroll->setStartValue(_scroll->endValue());
    _scroll->setCurrentTime(0);
    _scroll->setDuration(200);
}

void scrollBar::updateHandler()
{
    double h = _size.height() - _size.height() / 5 * sqrt(valueRange);
    if (h < 20) h = 20;

    deltaHeight = _size.height() - h;

    handlerBuf = QRectF(_size.width() - 8,
                        valueRange == 0 ? 0 : deltaHeight / valueRange * _value,
                        8,
                        h);
}
