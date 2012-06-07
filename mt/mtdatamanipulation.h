#ifndef DATAMANIPULATION_H
#define DATAMANIPULATION_H
#include <QtCore>
class MtTemplateItem;
class MtTemplate;
class QWidget;
class MtIndicatorItem;
class MtCompare;
class MtIndicatorConnection;
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
        virtual int state() const;


        MtIndicatorItems indicators() const;

        virtual void save();
        virtual void load();
        const MtTemplateItem * parent() const;

        MtIndicatorConnection* connectIndicator(MtIndicatorItem* indicator,MtCompare* comparer);
        void disconnectIndicator(MtIndicatorItem* indicator);
        void disconnectAllIndicators();
        virtual void setupConnection(MtIndicatorConnection* connection);
       private:
        typedef QSet<MtIndicatorConnection*> MtIndicatorConnections;
        MtIndicatorConnections m_connections;
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
        MtIndicatorItem(MtTemplateItem * parent);
        ~MtIndicatorItem();
        void disconnect();
        MtDataItem* sourceItem();

        int state() const;
        bool isIndicator() const;
        bool isReadOnly() const;

        void setupConnection(MtIndicatorConnection *connection);
private:
        MtIndicatorConnection* m_connection;
};

#endif // DATAMANIPULATION_H
