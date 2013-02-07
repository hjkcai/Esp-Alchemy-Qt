#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>
#include <QImage>
#include <Qlist>

class elements;

class element
{
public:
    QImage icon() const { return _icon; }
    QString id() const { return _id;}
    bool isBasicElement() const { return _basic; }
    bool isTerminalElement() const { return _terminal; }
    QString name() const { return _name; }

    static elements allElements;
    static element create(const QString &id, const QImage &icon, const QString &name, const bool &terminal, const bool &basic);

    //bool mark;

private:
    explicit element() { }

    QString _id;
    QImage _icon;
    QString _name;
    bool _terminal;
    bool _basic;

    friend class elements;
    friend class elementItem;
};

class elements : public QList<element>
{
public:
    int findElementByID(const QString &id, const int &start = 0) const;
    int findElementByName(const QString &name, const int &start = 0) const;
};

void initializeAllElements();

#endif // ELEMENT_H
