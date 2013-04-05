#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <QtCore>
#include "gamewidget.h"

class achievements;

class achievementData
{
public:
    static elementItems *known_elements;
    static QList<int> *known_combinations;
    static elementItems *workspace;
};

typedef bool (*achievementCheckDelegate)();

class achievement : public QObject
{
    Q_OBJECT
    friend class achievements;

public:
    explicit achievement(const QString &name = "", const QString &description = "", const achievementCheckDelegate &check = 0, const int &reward = 0, const QDateTime &achieveDate = QDateTime());

    QString name() const { return _name; }
    bool check();
    int reward() const { return _reward; }
    bool isAchieved() const { return _achieved; }
    QDateTime achieveDate() const { return _achieveDate; }

    void setAchieveDate(const QDateTime &achieveDate) { _achieveDate = achieveDate; }

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
    void reset();
};

void initializeAllAchievements();

#endif // ACHIEVEMENT_H
