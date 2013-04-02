#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include <QtGui>

class rectangleItem : public QGraphicsRectItem
{
public:
    explicit rectangleItem(QGraphicsItem *parent) : QGraphicsRectItem(parent) { }
    
    int radius() const { return _radius; }
    void setRadius(const int &value) { _radius = value; this->update(); }
    
private:
    int _radius;
    
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

#endif // RECTANGLEITEM_H
