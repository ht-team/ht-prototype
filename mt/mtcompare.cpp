#include "mtcompare.h"
#include <QList>
#include <QMutex>
static QMutex* s_comparersLocker = 0;
static QList<MtCompareHandler*>* s_comparers = 0;

static void InitState()
{
    if(!s_comparersLocker)
        s_comparersLocker = new QMutex();
    if(!s_comparers)
        s_comparers = new QList<MtCompareHandler*>();
}

MtCompare::~MtCompare()
{
}

MtCompare * MtCompare::tester(const QString &code)
{
    InitState();
    QMutexLocker ml(s_comparersLocker);
    MtCompare * tester=0;
    foreach(MtCompareHandler * h, *s_comparers)
    {

        if((tester=h->create(code)))
        {
            break;
        }
    }
    return tester;
}

MtCompareHandler::MtCompareHandler()
{
    InitState();
    QMutexLocker ml(s_comparersLocker);
    s_comparers->push_back(this);
}

MtCompareHandler::~MtCompareHandler()
{
    InitState();
    QMutexLocker ml(s_comparersLocker);
    s_comparers->removeAll(this);
}



