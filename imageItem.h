#ifndef __Alchemy__imageItem__
#define __Alchemy__imageItem__

#include <QtGui>
#include "graphics.h"

class imageItem : public graphicsItemBase
{
public:
    enum PaintMode
    {
        normal,
        centerScale,
        tile,
        scaledTile
    };

    explicit imageItem(QGraphicsItem *parent) : graphicsItemBase(parent) { }
    explicit imageItem(const QImage &image, QGraphicsItem *parent);
    
    QRectF boundingRect() const;
    
    QImage image() const { return _image; }
    void setImage(const QImage &value)
    {
        _image = value;
        if (_size.isEmpty()) _size = _image.size();
        this->update();
    }
    
    PaintMode paintMode() const { return _paintMode; }
    void setPaintMode(const PaintMode &value) { _paintMode = value; this->update(); }

    QSizeF size() const { return _size; }
    void setSize(const QSizeF &value) { _size = value; this->update(); }
    void setSize(const double &w, const double &h) { _size = QSizeF(w, h); this->update(); }

private:
    QImage _image;
    PaintMode _paintMode;
    QSizeF _size;
    
protected:
    void paintEvent(QPainter *painter);
};

#endif // __Alchemy__imageItem__
