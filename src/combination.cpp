#include "combination.h"
#include <QtCore>
#include "global.h"

combinations combination::allCombinations;

bool combination::canCombine(const elements &target) const
{
    if (target.length() < 2) return false;

    QList<int> found;

    for (int i = 0; i < target.length(); i++)
    {
        int index = source.findElementByID(target[i].id());
        while (index != -1)
        {
            int foundIndex = found.indexOf(index);
            if (foundIndex != -1)
                index = source.findElementByID(target[i].id(), found[foundIndex] + 1);
            else
            {
                found << index;
                break;
            }
        }

        if (index == -1)
            return false;
    }

    return true;
}

int combinations::findCombination(const elements &target)
{
    for (int i = 0; i < this->length(); i++)
    {
        if (this->at(i).canCombine(target))
            return i;
    }
    
    return -1;
}

void initializeAllCombinations()
{
    QFile *source_data = new QFile(ResourcesDir.arg("combinations"));
    source_data->open(QFile::ReadOnly | QFile::Text);

    combination::allCombinations = readCombinations(source_data->readAll()/*read*/);
    source_data->close(); delete source_data;
}

combinations readCombinations(const QString &source)
{
    QStringList sources = source.split('\n');
    combinations ret;

    for (int j = 0; j < sources.count(); j++)
    {
        QString data = sources[j].trimmed();

        if (data[0] == '*' || data[0] == '#')
        {
            if (data[0] == '#') continue;
            data = data.mid(1);
        }

        QStringList product = subString(data, QString::null, "=").split('+', QString::SkipEmptyParts);
        QStringList source = subString(data, "=").split('+', QString::SkipEmptyParts);

        combination result;
        for (int i = 0; i < product.length(); i++)
            result.product << element::allElements[element::allElements.findElementByID(subString(product[i], QString::null, ":"))];

        for (int i = 0; i < source.length(); i++)
            result.source << element::allElements[element::allElements.findElementByID(source[i])];

        ret << result;
    }

    return ret;
}

int combine(const elements &source)
{
//    int i = combination::allCombinations.findCombination(source);
//    if (i == -1) return elements();
//    else return combination::allCombinations[i].product;
    return combination::allCombinations.findCombination(source);
}
