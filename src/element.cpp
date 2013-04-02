#include "element.h"
#include <QtCore>
#include "global.h"

elements element::allElements;

element element::create(const QString &id, const QImage &icon, const QString &name, const bool &terminal, const bool &basic)
{
    element ret;
    ret._id = id;
    ret._icon = icon;
    ret._name = name;
    ret._terminal = terminal;
    ret._basic = basic;

    return ret;
}

int elements::findElementByID(const QString &id, const int &start) const
{
    for (int i = start; i < this->length(); i++)
    {
        if (this->at(i)._id.toLower() == id.toLower())
        {
            return i;
        }
    }

    return -1;
}

int elements::findElementByName(const QString &name, const int &start) const
{
    for (int i = start; i < this->length(); i++)
    {
        if (this->at(i)._name.toLower() == name.toLower())
        {
            return i;
        }
    }

    return -1;
}

bool initializeAllElements()
{
    QFile *source_data = new QFile(ResourcesDir.arg("combinations"));

    if (!source_data->open(QFile::ReadOnly | QFile::Text))
        return false;

    QStringList datal = QString(source_data->readAll())/*read*/.split('\n', QString::SkipEmptyParts);

    for (int i = 0; i < datal.count(); i++)
    {
        QString data = datal[i].trimmed();

        QString id = subString(data, "*", ":");
        bool basic = false;

        if (id[0] == '#')
        {
            basic = true;
            id = id.mid(1);
        }

        QString name = subString(subString(data, ":", "="), QString::null, "+");
        bool terminal = data[0] == '*';
        QImage icon(ResourcesDir.arg(QString("%0.png").arg(id)));

        element::allElements << element::create(id, icon, name, terminal, basic);
    }

    source_data->close(); delete source_data;
    return element::allElements.count() != 0;
}
