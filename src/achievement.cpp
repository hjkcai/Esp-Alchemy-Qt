#include "achievement.h"

achievements achievement::allAchievements;
elementItems *achievementData::known_elements;
QList<int> *achievementData::known_combinations;
elementItems *achievementData::workspace;

achievement::achievement(const QString &name, const QString &description, const achievementCheckDelegate &check, const int &reward, const QDateTime &achieveDate) : QObject(0)
{
    _name = name;
    _description = description;
    _check = check;
    _reward = reward;
    _achieveDate = achieveDate;
    _achieved = _check();
}

bool achievement::check()
{
    if (!_achieved)
    {
        _achieved = _check();
        if (_achieved)
        {
            _achieveDate = QDateTime::currentDateTime();
            emit achieved();
        }
    }

    return _achieved;
}

void achievements::checkForAll()
{
    for (int i = 0; i < this->count(); i++)
    {
        (*this)[i]->check();
    }
}

void achievements::reset()
{
    for (int i = 0; i < this->count(); i++)
    {
        (*this)[i]->_check();
    }
}

// 第一次成功组合
bool a() { return achievementData::known_combinations->count() >= 1; }

// 测试用
bool b() { return achievementData::known_combinations->count() % 2 == 0; }

void initializeAllAchievements()
{
    achievement *_a = new achievement(achievement::tr("a.name"), achievement::tr("a.description"), a, 10);
    achievement *_b = new achievement(achievement::tr("b.name"), achievement::tr("b.description"), b, 5);

    achievement::allAchievements << _a << _b;
}
