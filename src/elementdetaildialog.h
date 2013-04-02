#ifndef ELEMENTDETAILDIALOG_H
#define ELEMENTDETAILDIALOG_H

#include "combination.h"
#include "element.h"
#include "graphics.h"
#include "imageItem.h"
#include "scrollbar.h"

class elementDetailDialog : public dialogBase
{
    Q_OBJECT
public:
    explicit elementDetailDialog(const element &target, const QList<int> &known_combinations, QGraphicsItem *parent = 0);
    
protected:
    void paintEvent(QPainter *p);
    void resizeEvent(QResizeEvent *);

private:
    const static QString red;

    imageItem *image;
    QList<int> known_combinations;
    QTextDocument *l;
    element target;
    scrollBar *scroll;

private slots:
    void scroll_valueChanged();
};

#endif // ELEMENTDETAILDIALOG_H
