#ifndef __Alchemy__helper__
#define __Alchemy__helper__

#include <QtGui>

void debug_out(const QString &msg);
void drawScaledImage(QPainter *painter, const QImage &image, const double &x, const double &y, const double &width, const double &height);
QString subString(const QString &s, const QString &after = QString::null, const QString &before = QString::null);

#endif // __Alchemy__combination__
