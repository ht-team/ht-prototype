#include "comparetest.h"
#include <QVariant>
#include <mtdatamanipulation.h>
static TestComparerHandler s_handler;

MtCompare *TestComparerHandler::create(const QString &code)
{
    if(code == "mt.compare.test")
    {
        return new CompareTest();
    }
    return 0;
}


int CompareTest::test(const MtDataItem *which)
{
    QVariant vWhich = which->dataView();
    return vWhich.toInt() > 1;
}
