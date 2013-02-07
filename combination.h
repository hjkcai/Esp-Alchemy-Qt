#ifndef COMBINATION_H
#define COMBINATION_H

#include "element.h"

// 元素组合集合
class combinations;

// 元素组合
class combination
{
public:
    explicit combination() { }

    // 判断 target 是否与 source，即是否能进行组合
    // 若能够组合，则返回true
    bool canCombine(const elements &target) const;
    
    // 源组合（反应物）
    elements source;

    // 生成组合（生成物）
    elements product;

    // 从数据文件中读取到的所有组合
    static combinations allCombinations;
};

class combinations : public QList<combination>
{
public:
    // 寻找是否存在能够匹配 target 的组合
    // 返回找到的组合的位置，若未找到则返回 -1
    int findCombination(const elements &target);
};

// 从数据文件中读取组合
void initializeAllCombinations();

combinations readCombinations(const QString &source);

// 组合元素，返回生成组合
int combine(const elements &source);

#endif // COMBINATION_H
