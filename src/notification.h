#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "graphics.h"
#include "imageItem.h"
#include "textItem.h"

class notification : public graphicsItemBase
{
    Q_OBJECT
public:
    explicit notification(QGraphicsItem *parent = 0);

    QImage icon() const { return _icon->image(); }
    void setIcon(const QImage &icon) { _icon->setImage(icon); }

    QString line1() const { return _line1->text(); }
    void setLine1(const QString &line1) { _line1->setText(QString("<b>%0</b>").arg(line1)); }

    QString line2() const { return _line2->text(); }
    void setLine2(const QString &line2) { _line2->setText(line2); }

    int achievementPoint() const { return _achi->text().toInt(); }
    void setAhievementPoint(const int &i) { _achi->setText(QString("%0").arg(i)); }

    void show();

signals:
    void close();

protected:
    void paintEvent(QPainter *p);
    void resizeEvent(QResizeEvent *);

private slots:
    void ctimer_timeout();

private:
    imageItem *_icon;
    textItem *_line1, *_line2, *_achi;

    QGraphicsDropShadowEffect *shadow;
    QPropertyAnimation *oa;
    QTimer *ctimer;     // close timer
};

#endif // NOTIFICATION_H
