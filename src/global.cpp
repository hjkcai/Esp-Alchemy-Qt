#include "global.h"
#include <algorithm>
using namespace std;

void drawScaledImage(QPainter *painter, const QImage &image, const double &x, const double &y, const double &width, const double &height)
{
    double scale = 0;
    double scaled_w, scaled_h;
    
    if (image.width() >= image.height())
    {
        scale = image.width() / (double)width;
        scaled_w = width;
        scaled_h = image.height() / scale;
    }
    else
    {
        scale = image.height() / (double)height;
        scaled_w = image.width() / scale;
        scaled_h = height;
    }
    
    QImage scaled = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter->drawImage(QPointF(x + max<double>(width, scaled_w) / 2.0 - min<double>(width, scaled_w) / 2.0,
                               y + max<double>(height, scaled_h) / 2.0 - min<double>(height, scaled_h) / 2.0),
                       scaled);
}

QString subString(const QString &s,const QString &after, const QString &before)
{
    int start = after.isNull() ? 0 : s.indexOf(after) + after.length();
    if (start == -1) start = 0;

    int end = before.isNull() ? s.length() : s.lastIndexOf(before) - 1;
    if (end == -1) end = s.length();

    return s.mid(start, end - start + 1);
}

QString ResourcesDir;
QString GameSaveDirParent, GameSaveFileName;
