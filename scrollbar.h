#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "graphics.h"

class scrollBar : public graphicsItemBase
{
    Q_OBJECT
    Q_PROPERTY(double drawOpacity READ drawOpacity WRITE setDrawOpacity)
    Q_PROPERTY(double value READ value WRITE setValue)

public:
    explicit scrollBar(QGraphicsItem *parent = 0);

    QRectF boundingRect() const;

    double drawOpacity() const { return _drawOpacity; }
    void setDrawOpacity(const double &value) { _drawOpacity = value; this->update(); }

    double maximum() const { return _max; }
    void setMaximum(const double &value);

    double minimum() const { return _min; }
    void setMinimum(const double &value);

    QSizeF size() const { return _size; }
    void setSize(const QSizeF &value)
    {
        _size = value;
        _update();
    }
    void setSize(const double &w, const double &h) { setSize(QSizeF(w, h)); }

    double value() const { return _value; }
    void setValue(const double &value);

protected:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void paintEvent(QPainter *p);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

private:
    double _max;
    double _min;
    QSizeF _size;
    double _value;
    double _drawOpacity;

    void _update()
    {
        updateHandler();
        this->update();
    }

    QPropertyAnimation *_fade;
    QPropertyAnimation *_scroll;

    QRectF handlerBuf;
    double deltaHeight;
    double valueRange;
    void updateHandler();

    bool _pressed;
    double _Yoffset;

    bool intersects(const QPointF &pos)
    { return handlerBuf.intersects(QRectF(pos, QSizeF(0.1, 0.1))); }

private slots:
    void _scroll_finished();

signals:
    void valueChanged();
};

#endif // SCROLLBAR_H
