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

    bool alwaysShow() const { return _alwaysShow; }
    void setAlwaysShow(const bool &value) { _alwaysShow = value; this->update(); }

    double drawOpacity() const { return _drawOpacity; }
    void setDrawOpacity(const double &value) { _drawOpacity = value; this->update(); }

    double maximum() const { return _max; }
    void setMaximum(const double &value);

    double minimum() const { return _min; }
    void setMinimum(const double &value);

    double value() const { return _value; }
    void setValue(const double &value);

protected:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void paintEvent(QPainter *p);
    void resizeEvent(QResizeEvent *e)
    {
        _update();
        graphicsItemBase::resizeEvent(e);
    }
    void wheelEvent(QGraphicsSceneWheelEvent *event);

private:
    double _max;
    double _min;
    double _value;
    double _drawOpacity;
    bool _alwaysShow;

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
