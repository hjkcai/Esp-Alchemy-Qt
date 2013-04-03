#include "textItem.h"

textItem::textItem(QGraphicsItem *parent) : graphicsItemBase(parent)
{
    initialize();
}

textItem::textItem(const QString &text, QGraphicsItem *parent) : graphicsItemBase(parent)
{
    initialize();
    setText(text);
}

void textItem::initialize()
{
    l = new QTextDocument();
    l->setUseDesignMetrics(true);

    shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(1);
    shadow->setColor(Qt::black);
    shadow->setOffset(1);

    this->setGraphicsEffect(shadow);
    setAlignment(Qt::AlignCenter);
    setSize(75, 23);
}

void textItem::paintEvent(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(0.5, 0.5);
    l->drawContents(painter, this->boundingRect());
}
