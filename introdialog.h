#ifndef INTRODIALOG_H
#define INTRODIALOG_H

#include "graphics.h"

class introDialog : public dialogBase
{
    Q_OBJECT
public:
    explicit introDialog(QGraphicsItem *parent = 0);

protected:
    void paintEvent(QPainter *p);

private:
    QTextDocument *l;
};

#endif // INTRODIALOG_H
