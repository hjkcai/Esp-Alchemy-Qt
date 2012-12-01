#include "element.h"
#include <QtCore>
#include "helper.h"

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
//            if (mark)
//            {
//                if (this->at(i).mark)
//                    continue;
//                else (*this)[i].mark = true;
//            }

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
//            if (mark)
//            {
//                if (this->at(i).mark)
//                    continue;
//                else this->at(i).mark = true;
//            }

            return i;
        }
    }

    return -1;
}

void initializeAllElements()
{
    QFile source_data(":/res/combinations.txt"); source_data.open(QFile::ReadOnly | QFile::Text);

    while (!source_data.atEnd())
    {
        QString data = QString(source_data.readLine()).trimmed();

        QString id = subString(data, "*", ":");
        bool basic = false;

        if (id[0] == '#')
        {
            basic = true;
            id = id.mid(1);
        }

        QString name = subString(subString(data, ":", "="), QString::null, "+");
        bool terminal = data[0] == '*';
        QImage icon(QString(":/res/%0.png").arg(id));

        element::allElements << element::create(id, icon, name, terminal, basic);
    }
}
