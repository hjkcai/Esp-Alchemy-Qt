#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QtGui>
#include "graphics.h"

class textItem : public graphicsItemBase
{
public:
    explicit textItem(QGraphicsItem *parent);
    explicit textItem(const QString &text, QGraphicsItem *parent);

    Qt::AlignmentFlag alignment() const { return _align; }
    void setAlignment(const Qt::AlignmentFlag &value) { _align = value; l->setDefaultTextOption(QTextOption(_align)); this->update(); }

    QString text() const { return _text; }
    void setText(const QString &value) { _text = value; l->setHtml(_text); this->update(); }

    QGraphicsDropShadowEffect *shadow;

private:
    QString _text;
    Qt::AlignmentFlag _align;
    QTextDocument *l;

    void initialize();

protected:
    void paintEvent(QPainter *painter);
    void resizeEvent(QResizeEvent *) { l->setTextWidth(_size.width()); }
};

#endif // __Alchemy__textItem__
