#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtGui>

class graphicsItemBase : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit graphicsItemBase(QGraphicsItem *parent) : QGraphicsObject(parent) { }

signals:
    void userPaint(QPainter *painter);

protected:
    virtual void paintEvent(QPainter *) { }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

class graphicsViewBase : public QGraphicsView
{
    Q_OBJECT
public:
    explicit graphicsViewBase(QGraphicsScene *scene, QWidget *parent = 0) : QGraphicsView(scene, parent) { }

signals:
    void mousePress(QMouseEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event);
};

class drawerGraphicsItem : public graphicsItemBase
{
    Q_OBJECT
public:
    explicit drawerGraphicsItem(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void updateBuffer(const QSizeF &size);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) { emit hoverEnter(event); }
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) { emit hoverLeave(event); }
    void paintEvent(QPainter *p);

signals:
    void hoverEnter(QGraphicsSceneHoverEvent *event);
    void hoverLeave(QGraphicsSceneHoverEvent *event);

private:
    static const int drawerSize = 300;
    QImage buffer, scaledImageBuffer, backgroundBuffer;
    QSizeF _size;
};

#endif // GRAPHICS_H
