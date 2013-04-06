#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGui>

void drawScaledImage(QPainter *painter, const QImage &image, const double &x, const double &y, const double &width, const double &height);
QString subString(const QString &s, const QString &after = QString::null, const QString &before = QString::null);

const int AnimationDuration = 200;
const double AnimationDurationF = (double)AnimationDuration;

extern QString ResourcesDir;
extern QString GameSaveDirParent, GameSaveFileName;

#endif // GLOBAL_H
