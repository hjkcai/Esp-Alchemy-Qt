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
    
    QImage image() const { return _image; }
    void setImage(const QImage &value)
    {
        _image = value;
        if (_size.isEmpty()) _size = _image.size();
        this->update();
    }
    
    PaintMode paintMode() const { return _paintMode; }
    void setPaintMode(const PaintMode &value) { _paintMode = value; this->update(); }

private:
    QImage _image;
    PaintMode _paintMode;
    
protected:
    void paintEvent(QPainter *painter);
};

#endif // __Alchemy__imageItem__
