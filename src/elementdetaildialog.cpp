#include "elementdetaildialog.h"

const QString elementDetailDialog::red = "<font color=\"red\">%0</font>";

elementDetailDialog::elementDetailDialog(const element &target, const QList<int> &known_combinations, QGraphicsItem *parent) :
    dialogBase(parent)
{
    this->target = target;
    this->known_combinations = known_combinations;

    image = new imageItem(this);
    image->setImage(this->target.icon());
    image->setPos(12, 12);
    image->setPaintMode(imageItem::centerScale);
    image->setSize(48, 48);

    scroll = new scrollBar(this);
    scroll->setAlwaysShow(false);
    scroll->setPos(72, 12 + 48);
    connect(scroll, SIGNAL(valueChanged()), this, SLOT(scroll_valueChanged()));

    QList<int> inSource, inProduct;
    for (int i = 0; i < known_combinations.count(); i++)
    {
        combination *c = &combination::allCombinations[known_combinations[i]];

        if (c->product.findElementByID(target.id()) != -1)
            inProduct << known_combinations[i];
        else if (c->source.findElementByID(target.id()) != -1)
            inSource << known_combinations[i];
    }

    inProduct.append(inSource);
    QString result;

    for (int i = 0; i < inProduct.count(); i++)
    {
        combination *c = &combination::allCombinations[inProduct[i]];
        for (int j = 0; j < c->product.count(); j++)
        {
            if (c->product[j].id() == target.id())
                result.append(red.arg(c->product[j].name()));
            else result.append(c->product[j].name());

            if (j != c->product.count() - 1)
                result.append(" + ");
        }

        result.append(" = ");
        for (int j = 0; j < c->source.count(); j++)
        {
            if (c->source[j].id() == target.id())
                result.append(red.arg(c->source[j].name()));
            else result.append(c->source[j].name());

            if (j != c->source.count() - 1)
                result.append(" + ");
        }

        if (i != inProduct.count() - 1)
            result.append("<br>");
    }

    l = new QTextDocument();

    if (result.trimmed().isEmpty())
        result = tr("<font color=\'blue\'>There is no combinations found<br>that is related to this element!<br>Continue playing and find them!</font>");
    else
    {
        QFont f = l->defaultFont();
#ifdef Q_OS_MAC
        f.setPointSizeF(12);
#else
        f.setPointSizeF(10.5);
#endif
        l->setDefaultFont(f);
    }

    l->setHtml(result);
    l->setDocumentMargin(0);

    int height = (int)l->size().height() + (12 + 12 + 48 + 8);
    if (height > 300)
    {
        scroll->setMaximum((height - 300) / 10.0);
        height = 300;
    }
    this->setSize(300, height);
}

void elementDetailDialog::paintEvent(QPainter *p)
{
    dialogBase::paintEvent(p);

    QFont f = p->font(), f1 = p->font();
#ifdef Q_OS_MAC
    f.setPointSize(18);
#else
    f.setPointSize(16);
#endif

    p->setFont(f);
    p->setPen(QColor(Qt::black));
    p->drawText(QRectF(72, 12, _size.width() - (72 + 12), 48), target.name(), QTextOption(Qt::AlignVCenter));

    p->setFont(f1);
    p->translate(72, 12 + 48 - (scroll->value() * 10));
    l->drawContents(p, QRectF(0, scroll->value() * 10, _size.width() - (72 + 12 - 12/*scroll*/), _size.height() - (12 + 12 + 48 + 8)));
}

void elementDetailDialog::resizeEvent(QResizeEvent *)
{
    scroll->setSize(_size.width() - (72 + 12), _size.height() - (12 + 12 + 48 + 8));
}

void elementDetailDialog::scroll_valueChanged()
{
    this->update();
}
