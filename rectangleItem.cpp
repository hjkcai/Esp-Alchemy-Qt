#include "rectangleItem.h"

void rectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(0.5, 0.5);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    painter->drawRoundedRect(this->rect(), _radius, _radius);
}