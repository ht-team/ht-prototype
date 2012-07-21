#include "comparetest.h"
#include <QVariant>
#include <mtdatamanipulation.h>
static TestComparerHandler s_handler;

MtCompare *TestComparerHandler::create(const QString &code)
{
    if  (code == "logical compare")
    {
        return new CompareLogical();
    }

    /*if  (code == "number range")
    {
        return new CompareRange();
    }

    if  (code == "values equality")
    {
        return new CompareEqual();
    }

    if  (code == "larger or equaly")
    {
        return new CompareLagerEqualy();
    }

    if  (code == "less or equal")
    {
        return new CompareLessEqualy();
    }

    if  (code == "larger")
    {
        return new CompareLager();
    }

    if  (code == "less")
    {
        return new CompareLess();
    }*/

    return 0;
}


int CompareLogical::test(const MtDataItem *which)
{
    qDebug()<<"come here";
    QVariantList values = which->data();

    if ( values.at(5).toInt() != 1 )
        return  0;

    //если данные не введены
    if ( ( values.at(1).isNull() ) && ( values.at(1).toString() == "" ) )

        return  MtCompare::Other;

    //сравнение фактического значения с нормой
    if ( values.at(1).toInt() == values.at(3).toInt() )
        return MtCompare::Ready;
    else
        return MtCompare::NotReady;
}
