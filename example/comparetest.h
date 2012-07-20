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

class CompareLogical : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

class CompareRange : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

class CompareEqual : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

class CompareLagerEqualy : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

class CompareLessEqualy : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

class CompareLager : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

class CompareLess : public MtCompare
{
public:
    int test(const MtDataItem *which);
private:

};

#endif // COMPARETEST_H
