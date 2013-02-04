#include "textItem.h"

textItem::textItem(QGraphicsItem *parent) : graphicsItemBase(parent)
{
    l = new QTextDocument();
    l->setUseDesignMetrics(true);
    l->setDefaultTextOption (QTextOption(Qt::AlignCenter));

    setSize(75, 23);
}

textItem::textItem(const QString &text, QGraphicsItem *parent) : graphicsItemBase(parent)
{
    l = new QTextDocument();
    l->setUseDesignMetrics(true);
    l->setDefaultTextOption(QTextOption(Qt::AlignCenter));

    setText(text);
    setSize(75, 23);
}

void textItem::paintEvent(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(0.5, 0.5);
    l->drawContents(painter, this->boundingRect());
}
