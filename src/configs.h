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

    QList<QString> getValue(const QString &key) const;
    void setValue(const QString &key, const QList<QString> &value);

private:
    QString _filename;
    QList<QString> keys;
    QList<QList<QString> > values;
};

#endif // CONFIGS_H
