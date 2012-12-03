#include "inappdialog.h"

InAppDialog::InAppDialog(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
    this->setStyleSheet("QWidget{border:1px solid white;border-radius:5px;background-color:transparent}");

#ifndef Q_OS_MAC
    _shadow = new QGraphicsDropShadowEffect(this);
    _shadow->setBlurRadius(5);
    _shadow->setColor(Qt::black);
    _shadow->setOffset(0, 10);
    this->setGraphicsEffect(_shadow);
#endif
}

void InAppDialog::closeDialog(const DialogResult &result)
{
    this->close();
    emit DialogClosed(result);
}
