#ifndef CONFIGS_H
#define CONFIGS_H

#include <QtCore>

// todo: 加入haffman编码

class configs
{
public:
    explicit configs(const QString &filename = "");
    
    bool load(const QString &filename);
    void save(const QString &filename = "");

    QString getValue(const QString &key, const int &index = 0) const;
    void setValue(const QString &key, const int &index, const QString &value);

    QString _filename;
    QList<QString> keys;
    QList<QList<QString> > values;
};

#endif // CONFIGS_H
