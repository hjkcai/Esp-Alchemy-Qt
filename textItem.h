#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QtGui>
#include "graphics.h"

class textItem : public graphicsItemBase
{
public:
    explicit textItem(QGraphicsItem *parent);
    explicit textItem(const QString &text, QGraphicsItem *parent);
        
    QString text() const { return _text; }
    void setText(const QString &value) { _text = value; l->setHtml(_text); this->update(); }
    
private:
    QString _text;
    QTextDocument *l;

protected:
    void paintEvent(QPainter *painter);
    void resizeEvent(QResizeEvent *) { l->setTextWidth(_size.width()); }
};

#endif // __Alchemy__textItem__
