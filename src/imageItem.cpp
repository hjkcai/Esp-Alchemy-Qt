#include "imageItem.h"
#include "global.h"
#include <algorithm>
#include <cmath>
using namespace std;

imageItem::imageItem(const QImage &image, QGraphicsItem *parent) : graphicsItemBase(parent)
{
    _image = image;
}

void imageItem::paintEvent(QPainter *painter)
{
    switch (this->_paintMode)
    {
    case imageItem::normal:
        painter->drawImage(0, 0, this->_image);
        break;
    case imageItem::centerScale:
        drawScaledImage(painter, this->_image, 0, 0, this->_size.width(), this->_size.height());
        break;
    default:
        QImage nimage;
        if (this->paintMode() == imageItem::scaledTile)
            nimage = _image.scaled(min<double>(_size.width(), _size.height()), min<double>(_size.width(), _size.height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        else nimage = _image;

        int cw = ceil(_size.width() / nimage.width()),
            ch = ceil(_size.height() / nimage.height());

        for (int i = 0; i < cw; i++)
        {
            for (int j = 0; j < ch; j++)
            {
                painter->drawImage(i * nimage.width(), j * nimage.height(), nimage);
            }
        }

        break;
    }
}
