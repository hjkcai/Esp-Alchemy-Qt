#include "elementItem.h"
#include "global.h"

const double HoverOpacity = 0.5;
const double PressedOpacity = 0.75;
const QString blackText = "<font align=\"center\" color=\"black\">%0</font>";
const QString whiteText = "<font align=\"center\" color=\"white\">%0</font>";

elementItem::elementItem(const element &base, QGraphicsItem *parent) : QGraphicsObject(parent)
{
    background_rect = new rectangleItem(this);
    background_rect->setOpacity(0);
    background_rect->setRect(this->boundingRect());
    background_rect->setRadius(5);
    
    icon = new imageItem(this);
    icon->setSize(48, 48);
    icon->setPaintMode(imageItem::centerScale);
    icon->setPos(8, 8);
    
    label = new textItem(this);
    label->setSize(64, 20);
    label->setPos(0, 60);
    
    shadow = new QGraphicsDropShadowEffect(label);
    shadow->setBlurRadius(1);
    shadow->setOffset(1);
    label->setGraphicsEffect(shadow);

    tl = new QTimeLine(AnimationDuration);
    tl->setFrameRange(0, AnimationDuration);
    connect(tl, SIGNAL(frameChanged(int)), this, SLOT(tl_frameChanged(int)));

    setDrawerStyle(false);
    _drag = true;
    _allowDrag = false;

    this->setBase(base);
    this->setAcceptHoverEvents(true);
}

QRectF elementItem::boundingRect() const
{
    return QRectF(0, 0, 64, 84);
}

QPainterPath elementItem::shape() const
{
    QPainterPath path;
    path.addRect(8, 8, 48, 48);
    return path;
}

void elementItem::setBase(const element &value)
{
    _base = value;
    icon->setImage(_base.icon());
    label->setText(blackText.arg(_base.name()));
}

void elementItem::setDrawerStyle(const bool &value)
{
    _drawerStyle = value;
    background_rect->setPen(QPen(Qt::transparent));
    label->setText(whiteText.arg(_base.name()));

    if (_drawerStyle)
    {
        background_rect->setBrush(QColor(29, 82, 255));
        shadow->setColor(Qt::black);
    }
    else
    {
        background_rect->setBrush(QColor(160, 160, 160));
        shadow->setColor(Qt::white);
    }
}

void elementItem::fadeBackgroundRect(const double &start, const double &to)
{  
    fade_start = start;
    fade_to = to;
    
    if (tl->state() == QTimeLine::Running)
    {
        tl->stop();
        fade_start = background_rect->opacity();
    }
    tl->start();
}

void elementItem::tl_frameChanged(int msec)
{
    background_rect->setOpacity(fade_start + msec / AnimationDurationF * (fade_to - fade_start));
}

void elementItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    fadeBackgroundRect(0, HoverOpacity);
}

void elementItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    fadeBackgroundRect(HoverOpacity, 0);
}

void elementItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    emit mousePressed(e);
    _allowDrag = false;

    if (e->button() != Qt::LeftButton) return;

    if (_drag)
    {
        _press = this->mapToScene(e->pos()).toPoint(); _press.null = false;
        p_press = this->pos().toPoint(); _press.null = false;
        _allowDrag = true;
    }

    fadeBackgroundRect(HoverOpacity, PressedOpacity);
}

void elementItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (_press.null || p_press.null || !_allowDrag || !_drag) return;

    QPoint cur = this->mapToScene(e->pos()).toPoint();
    int offx = cur.x() - _press.x();
    int offy = cur.y() - _press.y();
    
    this->setPos(p_press.x() + offx, p_press.y() + offy);
}

void elementItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    emit mouseReleased(e);

    if (e->button() != Qt::LeftButton) return;

    if (_drag)
    {
        _press.null = true;
        p_press.null = true;
    }

    fadeBackgroundRect(PressedOpacity, HoverOpacity);
}

int elementItems::findItemByID(const QString &id, const int &start) const
{
    for (int i = start; i < this->length(); i++)
    {
        if (this->at(i)->base().id().toLower() == id.toLower())
            return i;
    }

    return -1;
}

void elementItems::bringToFront(const int &index)
{
    if (index < 0 || index >= this->length()) return;

    int cur_z = this->at(index)->zValue();

    for (int i = 0; i < this->length(); i++)
    {
        if (this->at(i)->zValue() > cur_z)
            this->at(i)->setZValue(this->at(i)->zValue() - 1);

        if (i == index) this->at(i)->setZValue(this->length() - 1);
    }
}

elementItems elementItems::collidesItems(elementItem *item, const bool &include_self)
{
    int index = this->indexOf(item);
    if (index < 0 || index >= this->length()) return elementItems();

    elementItems ret;

    for (int i = 0; i < this->length(); i++)
    {
        if (include_self && i == index) ret << item;
        else if (this->at(i)->collidesWithItem(item))
        {
            ret << this->at(i);
        }
    }

    return ret;
}

void elementItems::remove(elementItem *item)
{
    int index = this->indexOf(item);
    if (index < 0 || index >= this->length()) return;

    int cur_z = item->zValue();

    for (int i = 0; i < this->length(); i++)
    {
        if (i == index) continue;
        if (this->at(i)->zValue() > cur_z)
            this->at(i)->setZValue(this->at(i)->zValue() - 1);
    }

    this->removeAt(index);
}

elements elementItems::toElements()
{
    elements ret;
    for (int i = 0; i < this->length(); i++)
        ret << this->at(i)->base();

    return ret;
}
