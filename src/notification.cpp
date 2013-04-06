#include "notification.h"
#include "global.h"

notification::notification(QGraphicsItem *parent) : graphicsItemBase(parent)
{
    _icon = new imageItem(this);
    _icon->setPos(6, 6);
    _icon->setPaintMode(imageItem::centerScale);

    _line1 = new textItem(this);
    _line1->setAlignment(Qt::AlignLeft);
    _line1->setPos(52, 8);
    _line1->shadow->setEnabled(false);

    _line2 = new textItem(this);
    _line2->setAlignment(Qt::AlignLeft);
    _line2->setPos(52, 26);
    _line2->shadow->setEnabled(false);

    _achi = new textItem(this);
    _achi->shadow->setEnabled(false);

    shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 5);

    this->setGraphicsEffect(shadow);
    this->setOpacity(0);
    this->setSize(300, 64);
}

void notification::show()
{
    ctimer = new QTimer(this);
    ctimer->setInterval(5000);
    connect(ctimer, SIGNAL(timeout()), this, SLOT(ctimer_timeout()));

    oa = new QPropertyAnimation(this, "opacity", this);
    oa->setDuration(AnimationDuration * 1.5);
    oa->setStartValue(0);
    oa->setEndValue(1);

    oa->start();
    ctimer->start();
}

void notification::paintEvent(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->translate(0.5, 0.5);

    p->setPen(QColor(Qt::transparent));
    p->setBrush(QColor(255, 255, 255, 242));
    p->drawRoundedRect(0, 0, size().width() - 1, size().height() - 1, 5, 5);

    QLinearGradient darkn(0, 0, 0, size().height());
    darkn.setColorAt(0, QColor(0, 0, 0, 0));
    darkn.setColorAt(1, QColor(0, 0, 0, 10));

    p->setBrush(darkn);
    p->drawRoundedRect(0, 0, size().width() - 1, size().height() - 1, 5, 5);

    p->setPen(QColor(136, 136, 136, 192));
    p->setBrush(QColor(Qt::transparent));
    p->drawRoundedRect(0, 0, size().width() - 1, size().height() - 1, 5, 5);
}

void notification::resizeEvent(QResizeEvent *)
{
    _icon->setSize(this->size().height() - 12, this->size().height() - 12);
    _line1->setSize(this->size().width() - 64, 24);
    _line2->setSize(this->size().width() - 64, 24);

    _achi->setPos(this->size().width() - 40, this->size().height() / 2 - 12);
    _achi->setSize(32, 18);
}

void notification::ctimer_timeout()
{
    ctimer->stop();

    QPropertyAnimation *out = new QPropertyAnimation(this, "pos", this);
    out->setDuration(AnimationDuration * 2);
    out->setStartValue(this->pos());
    out->setEndValue(QPointF(-this->size().width() - 20, this->pos().y()));

    oa->setDirection(QPropertyAnimation::Backward);
    oa->start();
    out->start();
    emit close();
}
