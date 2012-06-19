#include "exampletempateserrialization.h"
#include <QDateTime>
#include <mtcompare.h>
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
    t->setColumnCount(4);
    t->setTemplateType(MtTemplateFactory::JustTemplate);
    buildTemplateTree(t);
    emit end();
    return t;
}

void ExampleTempateSerrialization::buildTemplateTree(MtTemplate *t)
{
    int headerCount = 1;
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
    }

}

void ExampleTempateSerrialization::buildHeaderTree(MtHeader *header, int recLevel)
{
    int headerCount = 1;
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

    }

}



MtTemplateItem::ItemData ExampleTemplateFactory::
defaultHeaderData(MtTemplateFactory::TemplateType type, MtHeader *parent)
{
    MtTemplateItem::ItemData data;
    switch(type)
    {
    case JustTemplate:
        data.push_back(new MtDataItem(parent));
        data.push_back(new MtDataItem(parent));
    case EditableDocument:
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

        data.push_back(new MtDataItem(parent));
    }
    case EditableDocument:

        MtDataItem* dataItem = new ExampleCustomDataItem(parent);
        MtIndicatorItem* indicator = new MtIndicatorItem(parent);
        dataItem->connectIndicator(indicator, MtCompare::tester("mt.compare.test"));
        data.push_back(dataItem);
        data.push_back(indicator);
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

        data.push_back(new MtDataItem(parent));
    }
    case EditableDocument:

        data.push_back(new MtDataItem(parent));
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
    if(data.size())
    {
        data[0] = "Loaded";
    }
    else
    {
        data << "Loaded";
    }

    this->setData(data);
    qDebug()<< "Was loaded" << this->data();
}
