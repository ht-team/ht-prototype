#ifndef EXAMPLETEMPATESERRIALIZATION_H
#define EXAMPLETEMPATESERRIALIZATION_H

#include "mtdatamanipulation.h"
#include "mttemplate.h"
class ExampleCustomDataItem : public MtDataItem
{
public:
    ExampleCustomDataItem(MtTemplateItem* parent);
    void save();
    void load();
};

class ExampleTempateSerrialization :
        public MtTemplateSerialization
{
    Q_OBJECT
public:
    explicit ExampleTempateSerrialization(QObject *parent = 0);
    void saveTemplate(MtTemplate *mtTemplate);
    MtTemplate * loadTemplate();
    
private:
    void buildTemplateTree(MtTemplate * t);
    void buildHeaderTree(MtHeader * header, int recLevel);

    QString visualData(const QVariantList &list);
    
};

#endif // EXAMPLETEMPATESERRIALIZATION_H
