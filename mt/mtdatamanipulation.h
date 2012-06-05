#ifndef DATAMANIPULATION_H
#define DATAMANIPULATION_H
#include <QtCore>
class MtTemplateItem;
class MtTemplate;
class QWidget;
class MtIndicatorItem;
class MtComparer;
/**
 *@class MtTemplateSerialization
 *Предоставляет обьект который знает как загрузить/сохранить шаблон
*/
class MtTemplateSerialization:public QObject
{
    Q_OBJECT
signals:
    void begin();
    void error(int code);
    void end();
    public:
    MtTemplateSerialization(QObject * parent=0);
        /*pure*/virtual MtTemplate *
    loadTemplate()=0;
        /*pure*/virtual void
    saveTemplate(MtTemplate * mtTemplate) =0;
};
/**
 *@class MtDataItem
 *Одна отображаемая ячейка данных
*/
class MtDataItem
{
     public:
        MtDataItem(MtTemplateItem * parent);
        typedef QList<MtIndicatorItem * > MtIndicatorItems;
        typedef QList<MtDataItem *> MtDataItems;
        virtual ~MtDataItem();

        const QVariantList & data() const;
        void setData(const QVariantList & data);
        virtual QVariant dataView() const;

        virtual bool isReadOnly() const ;
        virtual bool isIndicator() const;


        const MtIndicatorItems  & indicators() const;
        void setIndicators(const MtIndicatorItems & indicators);

        virtual void save();
        virtual void load();
        const MtTemplateItem * parent() const;
        MtIndicatorItems  & indicators();
       private:
        MtIndicatorItems m_indicators;
        MtTemplateItem * m_parent;
        QVariantList m_data;
};
/**
 *@class MtReadOnlyItem
 *ячейка данных только для чтения
*/
class MtReadOnlyItem:public MtDataItem
{
public:
    MtReadOnlyItem(MtTemplateItem * parent);
    bool isReadOnly() const;
};
/**
 *@class MtIndicatorItem
 *Индикатор,ячейка данных, чье значение зависит от данных в
 *другой ячейке, в том числе и в индикаторе
*/
class MtIndicatorItem:public MtDataItem
{
      public:
        MtIndicatorItem(MtTemplateItem * parent,MtDataItem * watch=0);
        const MtDataItems & watchers() const;
        void addWatcher(MtDataItem * watcher);
        void removeWatcher(MtDataItem * watcher);
        void update(MtDataItem * updateRequester);
        bool isIndicator() const;

        bool isReadOnly() const;
private:
        MtDataItems m_watchers;
};

#endif // DATAMANIPULATION_H
