#ifndef ELEMENTITEM_H
#define ELEMENTITEM_H

#include <QtGui>
#include "element.h"
#include "imageItem.h"
#include "rectangleItem.h"
#include "textItem.h"

class QPointN : public QPoint
{
public:
    QPointN() { null = false;}

    QPointN(const QPoint &p)
    {
        this->setX(p.x());
        this->setY(p.y());
        null = false;
    }

    bool null;
};

class elementItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit elementItem(QGraphicsItem *parent = 0) : QGraphicsObject(parent) { }
    explicit elementItem(const element &base, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;

    element base() const { return _base; }
    void setBase(const element &value);

    bool drawerStyle() const { return _drawerStyle; }
    void setDrawerStyle(const bool &value);

    bool drag() const { return _drag; }
    void setDrag(const bool &value) { _drag = value; }

signals:
    void mousePressed(QGraphicsSceneMouseEvent *e);
    void mouseReleased(QGraphicsSceneMouseEvent *e);

private:
    const static QString blackText;
    const static QString whiteText;

    element _base;
    rectangleItem *background_rect;
    imageItem *icon;
    textItem *label;
    QGraphicsDropShadowEffect *shadow;

    QPointN _press, p_press;
    QTimeLine *tl;
    double fade_start, fade_to;
    
    bool _drawerStyle;
    bool _drag;
    bool _allowDrag;

    void fadeBackgroundRect(const double &start, const double &to);
    
private slots:
    void tl_frameChanged(int msec);
    
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) { }
};

class elementItems : public QList<elementItem *>
{
public:
    int findItemByID(const QString &id, const int &start = 0) const;

    void bringToFront(const int &index);
    void bringToFront(elementItem *item) { bringToFront(this->indexOf(item)); }

    elementItems collidesItems(const int &index, const bool &include_self = false) { return collidesItems(this->at(index), include_self); }
    elementItems collidesItems(elementItem *item, const bool &include_self = false);

    void remove(const int &i) { remove(this->at(i)); }
    void remove(elementItem *item);

    elements toElements();
};


#endif // ELEMENTITEM_H
