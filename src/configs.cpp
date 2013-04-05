#include "configs.h"
#include "global.h"

configs::configs(const QString &filename)
{
    if (filename != "")
    {
        _filename = filename;
        load(_filename);
    }
}

bool configs::load(const QString &filename)
{
    QFile *file = new QFile(filename);
    if (!file->exists()) return false;

    if (!file->open(QFile::ReadOnly)) return false;
    keys.clear();
    values.clear();

    QByteArray _read;
    while (_read = file->readLine(), !_read.isEmpty())
    {
        QString read = _read.trimmed();
        keys << subString(read, QString::null, "=");

        QStringList value = subString(read, "=").split('|');
        QList<QString> vs;

        for (int i = 0; i < value.count() - 1; i++)
        {
            vs << value[i];
        }

        values << vs;
    }

    file->close();
    delete file;
    return true;
}

void configs::save(const QString &filename)
{
    QString fn = filename;
    if (fn == "")
    {
        fn = _filename;
        if (fn == "") return;
    }

    QFile *file = new QFile(fn);
    file->open(QFile::WriteOnly);

    QString data;
    for (int i = 0; i < keys.count(); i++)
    {
        data.append(keys[i]);
        data.append('=');
        for (int j = 0; j < values[i].count(); j++)
            data.append(QString("%0|").arg(values[i][j]));
        data.append('\n');
    }

    file->write(data.toAscii());
    file->waitForBytesWritten(3000);
    file->flush();
    file->close();
    delete file;
}

QString configs::getValue(const QString &key, const int &index) const
{
    int i = keys.indexOf(key);
    if (i == -1) return "";

    return values[i][index];
}

void configs::setValue(const QString &key, const int &index, const QString &value)
{
    int i = keys.indexOf(key);
    if (i == -1)
    {
        QList<QString> l;
        l << value;

        keys << key;
        values << l;
    }
    else values[i][index] = value;
}
