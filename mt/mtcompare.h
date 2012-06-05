#ifndef MTCOMPARE_H
#define MTCOMPARE_H
class QString;
class MtCompare;
class MtDataItem;
class MtCompareHandler
{
public:
    MtCompareHandler();
    ~MtCompareHandler();
    virtual MtCompare * create(const QString& code)=0;

};
class MtCompare
{
public:
    enum Result
    {
        Ready,
        NotReady,
        Other
    };
    static  MtCompare * tester(const QString& code);
    virtual Result test(const MtDataItem* which,const MtDataItem* with)=0;

};

#endif // MTCOMPARE_H
