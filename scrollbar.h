#ifndef __Alchemy__ScrollBar__
#define __Alchemy__ScrollBar__

#include "graphics.h"

class ScrollBar : public graphicsItemBase
{
    Q_OBJECT
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(double value READ value WRITE setValue)

public:
    explicit ScrollBar(QGraphicsItem *parent = 0);

    QRectF boundingRect() const;

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
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
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

signals:
    
public slots:
    
};

#endif // __Alchemy__ScrollBar__
