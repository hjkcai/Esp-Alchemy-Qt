#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <QtCore>
#include "gamewidget.h"

class achievements;

class achievementData
{
public:
    static QString *username;
    static elementItems *known_elements;
    static QList<int> *known_combinations;
    static elementItems *workspace;
};

typedef bool (*achievementCheckDelegate)();

class achievement : public QObject
{
    Q_OBJECT
public:
    explicit achievement(const QString &name = "", const QString &description = "", const achievementCheckDelegate &check = 0, const int &reward = 0, const QDateTime &achieveDate = QDateTime());

    QString name() const { return _name; }
    bool check();
    int reward() const { return _reward; }
    bool isAchieved() const { return _achieved; }
    QDateTime achieveDate() const { return _achieveDate; }

    static achievements allAchievements;

signals:
    void achieved();

private:
    QString _name, _description;
    achievementCheckDelegate _check;
    int _reward;
    bool _achieved;
    QDateTime _achieveDate;
};

class achievements : public QList<achievement*>
{
public:
    void checkForAll();
};

void initializeAllAchievements();

#endif // ACHIEVEMENT_H
