#ifndef COMPARETEST_H
#define COMPARETEST_H
#include <mtcompare.h>
class TestComparerHandler : public MtCompareHandler
{
public:
    TestComparerHandler(): MtCompareHandler()
    {}
    MtCompare* create(const QString &code);
};

class CompareTest : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

#endif // COMPARETEST_H
