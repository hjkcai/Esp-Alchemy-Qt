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

class drawerGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit drawerGraphicsScene(QObject *parent = 0);
    void updateBuffer(const QSizeF &size);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    static const int drawerSize = 300;
    QImage buffer, scaledImageBuffer, backgroundBuffer;
};

class drawerGraphicsItem : public graphicsItemBase
{
    Q_OBJECT
public:
    explicit drawerGraphicsItem(QGraphicsItem *parent = 0) : graphicsItemBase(parent) { }
    QRectF boundingRect() const;
    void updateBuffer(const QSizeF &size);

protected:
    void paintEvent(QPainter *p);

private:
    static const int drawerSize = 300;
    QImage buffer, scaledImageBuffer, backgroundBuffer;
    QSizeF _size;
};

#endif // GRAPHICS_H
