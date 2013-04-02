#ifndef COMBINATION_H
#define COMBINATION_H

#include "element.h"

class combinations;

class combination
{
public:
    explicit combination() { }

    bool canCombine(const elements &target) const;
    
    // 源组合（反应物）
    elements source;

    // 生成组合（生成物）
    elements product;

    static combinations allCombinations;
};

class combinations : public QList<combination>
{
public:
    int findCombination(const elements &target);
};

void initializeAllCombinations();

combinations readCombinations(const QString &source);

int combine(const elements &source);

#endif // COMBINATION_H
