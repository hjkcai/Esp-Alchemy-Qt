#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtGui>

class graphicsItemBase : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QSizeF size READ size WRITE setSize)

public:
    explicit graphicsItemBase(QGraphicsItem *parent) : QGraphicsObject(parent) { }

    virtual QRectF boundingRect() const { return QRectF(QPointF(0, 0), _size); }

    QSizeF size() const { return _size; }
    void setSize(const QSizeF &value);
    void setSize(const double &w, const double &h) { setSize(QSizeF(w, h)); }

signals:
    void userPaint(QPainter *painter);
    void resized(QResizeEvent *);

protected:
    QSizeF _size;

    virtual void paintEvent(QPainter *) { }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    virtual void resizeEvent(QResizeEvent *e) { emit resized(e); }
};

// ----------

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

// ----------

class drawerGraphicsItem : public graphicsItemBase
{
    Q_OBJECT
public:
    explicit drawerGraphicsItem(QGraphicsItem *parent = 0);
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
};

// ----------

class dialogBase : public graphicsItemBase
{
    Q_OBJECT
public:
    enum DialogResult
    {
        None,
        OK,
        Cancel
    };

    explicit dialogBase(QGraphicsItem *parent = 0);

protected:
    void paintEvent(QPainter *p);

private:
    QGraphicsDropShadowEffect *_shadow;
};

// ----------

class mouseShield : public graphicsItemBase
{
    Q_OBJECT
public:
    explicit mouseShield(QGraphicsItem *parent = 0) : graphicsItemBase(parent) { this->setAcceptHoverEvents(true); }

signals:
    void mouseReleased();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *) { }
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *) { }
    void hoverMoveEvent(QGraphicsSceneHoverEvent *) { }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) { }
    void mousePressEvent(QGraphicsSceneMouseEvent *) { }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) { emit mouseReleased(); }
};

#endif // GRAPHICS_H
