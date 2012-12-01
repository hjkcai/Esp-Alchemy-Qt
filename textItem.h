#ifndef __Alchemy__textItem__
#define __Alchemy__textItem__

#include <QtGui>
#include "graphics.h"

class textItem : public graphicsItemBase
{
public:
    explicit textItem(QGraphicsItem *parent);
    explicit textItem(const QString &text, QGraphicsItem *parent);
    
    QRectF boundingRect() const;
    
    QString text() const { return _text; }
    void setText(const QString &value) { _text = value; l->setHtml(_text); this->update(); }
    
    QSizeF size() const { return _size; }
    void setSize(const QSizeF &value) { _size = value; l->setTextWidth(_size.width()); this->update(); }
    void setSize(const double &w, const double &h) { setSize(QSizeF(w, h)); }
    
private:
    QString _text;
    QSizeF _size;
    
    QTextDocument *l;

protected:
    void paintEvent(QPainter *painter);
};

#endif // __Alchemy__textItem__
