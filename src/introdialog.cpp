#include "introdialog.h"

introDialog::introDialog(QGraphicsItem *parent) : dialogBase(parent)
{
    l = new QTextDocument();
    l->setTextWidth(300);
    l->setHtml(QString("<p style=\"font-size:16pt\">%0</p><p style=\"font-size:10.5pt;%1\">%2</p>").arg(tr("Welcome to Alchemy!")).arg(tr("color:red")).arg(tr("Translation file is missing!")));
    this->setSize(324, l->size().height() + 24);
}

void introDialog::paintEvent(QPainter *p)
{
    dialogBase::paintEvent(p);
    p->translate(12, 12);
    l->drawContents(p, QRectF(0, 0, l->size().width(), l->size().height()));
}
