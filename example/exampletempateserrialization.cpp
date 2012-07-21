#include "exampletempateserrialization.h"
#include <QDateTime>
#include <mtcompare.h>
#include <mtdatamanipulation.h>

#include <mtdatamanipulation.h>
#include <mttemplate.h>

#include <QDebug>
class ExampleTemplateFactory: public MtTemplateFactory
{
public:
    MtTemplateItem::ItemData
    defaultHeaderData(TemplateType type, MtHeader* parent);
    MtTemplateItem::ItemData
    defaultSubHeaderData(TemplateType type, MtSubHeader* parent);
    MtTemplateItem::ItemData
    defaultFooterData(TemplateType type, MtFooter *parent);
};

ExampleTempateSerrialization::ExampleTempateSerrialization(QObject *parent) :
    MtTemplateSerialization(parent)
{
}

void ExampleTempateSerrialization::saveTemplate(MtTemplate *mtTemplate)
{
    Q_UNUSED(mtTemplate)
}

MtTemplate *ExampleTempateSerrialization::loadTemplate()
{
    emit begin();
    MtTemplateFactory * factory =
            new ExampleTemplateFactory();
    MtTemplate * t =
            new MtTemplate(factory);
    t->setColumnCount(5);
    //t->setTemplateType(MtTemplateFactory::JustTemplate);
    t->setTemplateType(MtTemplateFactory::EditableDocument);
    buildTemplateTree(t);
    emit end();
    return t;
}

void ExampleTempateSerrialization::buildTemplateTree(MtTemplate *t)
{
    /*int headerCount = 1;
    int footerCount = 2;

    for(int i = 0; i < headerCount; i++)
    {
        MtHeader * h = t->addHeader();
        QString header = "Header "+QVariant(i).toString();
        h->itemData().at(0)->setData(QVariantList()<<header);
        buildHeaderTree(h,2);
    }

    for(int i = 0; i < footerCount; i++)
    {
       MtFooter * f = t->addFooter();
       f->itemData().at(0)->setData(QVariantList()<<"Footer");
       f->itemData().at(1)->setData(QVariantList()<<"Footer template");
    }*/


    const int headerCount = 1;
    const int footerCount = 2;

    const QString ListSignature[footerCount] =
    {"A.S. Petrov","P.P. Ivanov"};

    for(int i = 0; i < headerCount; i++)
    {
        MtHeader * h = t->addHeader();
        h->itemData().at(0)->setData(QVariantList()<<QVariant(i+1).toString());
        h->itemData().at(1)->setData(QVariantList()<<QString("Header ").append(QVariant(i+1).toString()));
        buildHeaderTree(h,2);
    }

    for(int i = 0; i < footerCount; i++)
    {
       MtFooter * f = t->addFooter();
       //name of signature
       f->itemData().at(0)->setData(QVariantList()<<QString("Footer Template ").append(QVariant(i).toString()));
       //signature
       f->itemData().at(1)->setData(QVariantList()<<ListSignature[i]);
    }
}

void ExampleTempateSerrialization::buildHeaderTree(MtHeader *header, int recLevel)
{
    /*int headerCount = 1;
    int footerCount = 2;
    int subheaderCount = 2;
    if(recLevel <= 0)
        return;

    for(int i = 0; i < headerCount; i++)
    {
        MtHeader * h = header->addHeader();
        QString headerStr = "Header "+QVariant(i).toString();
        QString sWhichHeader = QString("Header of "+
                header->itemData().at(0)->dataView().toString());
        h->itemData().at(0)->setData(QVariantList()<<headerStr);
        h->itemData().at(1)->setData(QVariantList()<<sWhichHeader);
        buildHeaderTree(h, recLevel-1);
    }

    for(int i = 0; i < subheaderCount; i++)
    {
        MtSubHeader * sh = header->addSubHeader();
        sh->itemData().at(0)->setData(QVariantList()<<"SubHeader(value > 2 = critical;\n >1 and <2 = can work;\n <1 = All right");
        sh->itemData().at(1)->setData(QVariantList()<<0.1);
    }

    for(int i = 0; i < footerCount; i++)
    {
        MtFooter * f = header->addFooter();
        f->itemData().at(0)->setData(QVariantList()<<"Footer");
        f->itemData().at(1)->setData(QVariantList()<<"Footer header");

    }*/


    const int headerCount = 1;
    const int footerCount = 2;
    const int subheaderCount = 6;

    if(recLevel <= 0)
        return;

    QString ListSignature[footerCount] =
    {"S.S. Sidorov","P.P. Pupkin"};

    //norma1, norma2, codeprov
    QVariant norma[subheaderCount][3] =
    {{1, NULL, 1}, //logical, "+"
     {1, NULL, 1}, //logical, "-"
     {1.2, 1.5, 2},//number range
     {1.0, 2.0, 2},//number range
     {5, NULL, 3},  // equal
     {6, NULL, 3}  // equal
     };

    //fact1`, fact2
    const QVariant fact[subheaderCount][2] =
    {{1, NULL},  //logical
     {0, NULL},  //logical
     {1.3, 1.4},//number range
     {3, NULL},  //number range
     {5, NULL},  // equal
     {5, NULL}   // equal
     };

    for(int i = 0; i < headerCount; i++)
    {
        MtHeader * h = header->addHeader();
        QString str = header->itemData().at(0)->data().at(0).toString()
                      +"."+QVariant(i+1).toString();
        //nompp
        h->itemData().at(0)->setData(QVariantList()<<str);
        //name
        h->itemData().at(1)->setData(QVariantList()<<QString("Header ").append(str));
        buildHeaderTree(h, recLevel-1);
    }

    for(int i = 0; i < subheaderCount; i++)
    {
        MtSubHeader * sh = header->addSubHeader();
        QString str =  header->itemData().at(0)->data().at(0).toString()
                       +"."+QVariant(i+1).toString();
        //nompp
        sh->itemData().at(0)->setData(QVariantList()<<str);
        //name
        sh->itemData().at(1)->setData(QVariantList()<<QString("SubHeader ").append(str));

        //norma - get dataview
        /*QVariantList v;
        v<<norma[i][0]
                <<norma[i][1]
                <<norma[i][2];*/
        str = visualData(QVariantList()<<norma[i][2]
                         <<norma[i][0]
                         <<norma[i][1]);
        //str = "xx";
        //dataview   norma1   norma2  codeprov
        sh->itemData().at(2)->setData(QVariantList()<<str
                                                    <<norma[i][0]
                                                    <<norma[i][1]
                                                    <<norma[i][2]);

        //fact - get dataview
        str = visualData(QVariantList()<<norma[i][2]
                                       <<fact[i][0]
                                       <<fact[i][1]);
        //add norma once again to compare values
        //dataview   fact1  fact2  norma1  norma2  codeprov
        sh->itemData().at(3)->setData(QVariantList()<<str
                                                    <<fact[i][0]
                                                    <<fact[i][1]
                                                    <<norma[i][0]
                                                    <<norma[i][1]
                                                    <<norma[i][2]);
    }

    for(int i = 0; i < footerCount; i++)
    {
        MtFooter * f = header->addFooter();
        QString str =  header->itemData().at(0)->data().at(0).toString()
                       +"."+QVariant(i).toString();
        //name of signature
        f->itemData().at(0)->setData(QVariantList()<<QString("Footer ").append(str));
        //signature
        f->itemData().at(1)->setData(QVariantList()<<ListSignature[i]);
    }

}



MtTemplateItem::ItemData ExampleTemplateFactory::
defaultHeaderData(MtTemplateFactory::TemplateType type, MtHeader *parent)
{
    MtTemplateItem::ItemData data;
    switch(type)
    {
    case JustTemplate: case EditableDocument:
    {
        /*data.push_back(new MtDataItem(parent));
        data.push_back(new MtDataItem(parent));*/

        //nompp
        data.push_back(new MtReadOnlyItem(parent));
        //name
        data.push_back(new MtReadOnlyItem(parent));
    }
    //case EditableDocument:
        break;
    }
    return data;
}

MtTemplateItem::ItemData ExampleTemplateFactory::defaultSubHeaderData(MtTemplateFactory::TemplateType type, MtSubHeader *parent)
{
    MtTemplateItem::ItemData data;
    switch(type)
    {
    case JustTemplate:
    {

        //data.push_back(new MtDataItem(parent));
        //nompp
        data.push_back(new MtReadOnlyItem(parent));
        //name
        data.push_back(new MtReadOnlyItem(parent));
        //norma
        data.push_back(new MtReadOnlyItem(parent));
    }
    break;
    case EditableDocument:
    {
        //nompp
        data.push_back(new MtReadOnlyItem(parent));
        //name
        data.push_back(new MtReadOnlyItem(parent));
        //norma
        data.push_back(new MtReadOnlyItem(parent));
        //fact value
        MtDataItem* dataItem = new ExampleCustomDataItem(parent);
        MtIndicatorItem* indicator = new MtIndicatorItem(parent);
        //dataItem->connectIndicator(indicator, MtCompare::tester("mt.compare.test"));
        data.push_back(dataItem);
        data.push_back(indicator);
    }
    break;
    }
    return data;
}

MtTemplateItem::ItemData ExampleTemplateFactory::defaultFooterData(MtTemplateFactory::TemplateType type, MtFooter *parent)
{
    MtTemplateItem::ItemData data;
    switch(type)
    {
    case JustTemplate:
    {

        //data.push_back(new MtDataItem(parent));
        //name of signature
        data.push_back(new MtReadOnlyItem(parent));
    }
    break;
    case EditableDocument:
    {

        //data.push_back(new MtDataItem(parent));
        //name of signature
        data.push_back(new MtReadOnlyItem(parent));
        //signature
        MtDataItem* dataItem = new ExampleCustomDataItem(parent);
        data.push_back(dataItem);
    }
    break;
    }
    return data;
}


ExampleCustomDataItem::ExampleCustomDataItem(MtTemplateItem *parent) : MtDataItem(parent)
{
}

void ExampleCustomDataItem::save()
{
    qDebug()<< "Was saved" << this->data();
}

void ExampleCustomDataItem::load()
{
    QVariantList data = this->data();
    /*if(data.size())
    {
        data[0] = "Loaded";
    }
    else
    {
        data << "Loaded";
    }

    this->setData(data);*/
    qDebug()<< "Was loaded" << this->data();

    if (data.size())
    {
        //codeprov
        switch (data.at(5).toInt())
        {
        case 1:
        {
              qDebug()<<"connect";
            MtIndicatorItem* indicator = new MtIndicatorItem(this->parent());
            this->connectIndicator(indicator, MtCompare::tester("logical compare"));

        }
        break;
        case 2:
        {
            MtIndicatorItem* indicator = new MtIndicatorItem(this->parent());
            this->connectIndicator(indicator, MtCompare::tester("number range"));
        }
        break;
        case 3:
        {
            MtIndicatorItem* indicator = new MtIndicatorItem(this->parent());
            this->connectIndicator(indicator, MtCompare::tester("values equality"));
        }
        break;
        }//end switch

    }
}

//---------------------------
//norma1, norma2, codeprov
//fact1, fact2, codeprov
QString ExampleTempateSerrialization::visualData(const QVariantList &list)
{
    //visual data depends of codeprov
    const QString symbols [7] = {"", " .. ", "", ">=", "<=", ">", "<"};

    if (list.count() != 3)
        return "";

    QString str = "";
    int codeprov = list.at(0).toInt();

    //if norma1 doesn't exist
    if ( list.at(1).toString() == "" )
    return str;

    switch (codeprov)
    {
    case 1:
    {
        //for norma
        if ( (list.at(1).toString() == "+")
        || (list.at(1).toString() == "-") )
        {
            str = list.at(1).toString();
        }
        //for fact values
        else
        {
            if ( qRound(list.at(1).toDouble()) == 0)
                str = "-";
            if ( qRound(list.at(1).toDouble()) == 1)
                str = "+";
        }
    }
    break;
    case 2:
    {
        str = list.at(1).toString()
             +symbols[codeprov-1]
             +list.at(2).toString();
    }
    break;
    case 3: case 4: case 5: case 6:
    {
        str = symbols[codeprov-1]
             +list.at(1).toString();
    }
    break;
    }//end switch

    return str;
}
