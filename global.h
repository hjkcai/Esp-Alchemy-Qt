#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGui>

void drawScaledImage(QPainter *painter, const QImage &image, const double &x, const double &y, const double &width, const double &height);
QString subString(const QString &s, const QString &after = QString::null, const QString &before = QString::null);

#ifndef ALCHEMY_CONSTS
#define ALCHEMY_CONSTS

// Global
const int AnimationDuration = 200;
const double AnimationDurationF = (double)AnimationDuration;

#endif

#endif // GLOBAL_H
