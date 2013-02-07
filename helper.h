#ifndef HELPER_H
#define HELPER_H

#include <QtGui>

void debug_out(const QString &msg);
void drawScaledImage(QPainter *painter, const QImage &image, const double &x, const double &y, const double &width, const double &height);
QString subString(const QString &s, const QString &after = QString::null, const QString &before = QString::null);

#endif // __Alchemy__combination__
