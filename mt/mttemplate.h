#ifndef MTTEMPLATE_H
#define MTTEMPLATE_H
#include <QtCore>
class MtTemplateItem;
class MtHeader;
class MtSubHeader;
class MtFooter;
class MtFooterHolder;
class MtTemplate;
class MtDataItem;
class MtTemplateFactory;


class MtTemplateHolder:public QObject
{
    Q_OBJECT
public:
    explicit MtTemplateHolder(QObject * parent);
    const int code() const;
    void setCode(int code);
    const QString & name() const;
    void setName(const QString & name);
    const QString & brief() const;
    void setBrief(const QString & brief);
    const int templateCode() const;
    void setTemplateCode(int code);
private:
    int m_code;
    QString  m_name;
    QString m_brief;
    int m_templateCode;

};
/**
 *@class MtTemplateItem
 *Базовая структурная единица шаблона, шаблон состоит из набора обьектов MtTemplateItem
 *и сам является таковым.
 *MtTemplateItem не занимается хранением данных, а несет информацию только о структуре шаблона,
 *для того чтобы получить/изменить данные нужно использовать метод itemData()
*/
class MtTemplateItem
{
    public:
    /**
     *@enum MtTemplateItem::Type
     *Описывает тип элемента шаблона
*/
    enum Type
    {
        Header,
        Footer,
        Subheader,
        Template
    };
    typedef QList<MtDataItem * > ItemData;
        MtTemplateItem(MtTemplate * parentTemplate);
        virtual ~MtTemplateItem();
        /**
         *@return Тип элемента шаблона
*/
        virtual int type() const=0;
        /**
         *Позволяет получить дочерний элемент
         *\param at индекс дочернего элемента
         *@return указатель на дочерний элемент или 0 если таковой не найден
*/
        MtTemplateItem * child(int at);
        /**
         * @overload child()
*/
        const MtTemplateItem * child(int at) const;

        bool removeChild(MtTemplateItem * child);
        bool removeChild(int at);

        bool swapChild(int from, int too);
        bool swapChild(MtTemplateItem* which,MtTemplateItem * with);
        bool swapChild(MtTemplateItem *which, int to);

        int  childCount() const;
        bool hasChilds() const;

        MtTemplateItem *parent();
        MtTemplate * parentTemplate() ;

        MtTemplate * toTemplate();
        MtHeader * toHeader();
        MtSubHeader * toSubHeader();
        MtFooter * toFooter();
        typedef QList<MtTemplateItem *> MtTemplateItems;
        MtTemplateItems & childs();
        const MtTemplateItems & childs()const;
        ItemData & itemData();
        const ItemData & itemData() const;
    protected:
        void setParent(MtTemplateItem * parent);
    private:
        MtTemplateItems m_childs;
        MtTemplateItem * m_parent;
        MtTemplate * m_parentTemplate;
        ItemData  m_itemData;

};

/**
 *@class MtTemplateFactory
 *Содержит алгоритмы, генерирующие набор данных по умолчанию,
 * для всех основных структурных единиц шаблона
*/
class MtTemplateFactory
{
public:
    enum TemplateType
    {
        JustTemplate,
        EditableDocument
    };
    virtual MtTemplateItem::ItemData
    defaultHeaderData(TemplateType type,MtHeader* parent) = 0;
    virtual MtTemplateItem::ItemData
    defaultSubHeaderData(TemplateType type,MtSubHeader* parent)= 0;
    virtual MtTemplateItem::ItemData
    defaultFooterData(TemplateType type,MtFooter* parent)= 0;

};
/**
 *@class MtFooterHolder
 *Структурная единица, у которой может быть подпись(MtFooter)
*/
class MtFooterHolder:public MtTemplateItem
{
    public:
        typedef QList<MtFooter*> MtFooters;
        MtFooterHolder(MtTemplate * parentTemplate);
        virtual ~MtFooterHolder();
        MtFooters & footers();
        const MtFooters & footers() const;
        MtFooter * addFooter();
    protected:
        MtTemplateItem *phyzicalHolder();
    private:
        MtFooters m_footers;
};

/**
 *@class MtTemplate
 *Представляет собой шаблон, является структурной единицей верхнего уровня,
 *не имеет родительской структурной единицы, може содержать в себе MtHeader, MtFooter.
*/
class MtTemplate:public MtFooterHolder
{
public:

    MtTemplate(MtTemplateFactory * factory);
    int columnCount() const;
    void setColumnCount(int columnCount);

    int type() const;

    MtHeader * addHeader();
    MtTemplateFactory * factory();
    MtTemplateFactory::TemplateType templateType()const;
    void setTemplateType(MtTemplateFactory::TemplateType type);
private:
    MtTemplateFactory * m_factory;
    int m_columns;
    MtTemplateFactory::TemplateType m_type;


};
/**
 *@class MtHeader
 *Заголовок, может содержатся в MtHeader, MtTemplate.
 *Может содержать MtHeader, MtSubHeader, MtFooter.
*/
class MtHeader:public MtFooterHolder
{
  public:
        MtHeader(MtTemplate* parent);
        MtHeader(MtHeader * parent);
        int type() const;

        MtHeader * addHeader();
        MtSubHeader * addSubHeader();
};

/**
 *@class MtSubHeader
 *Подзаголовок, может содержаться в MtHeader.
*/
class MtSubHeader:public MtTemplateItem
{
public:
    MtSubHeader(MtHeader * parent);
    int type() const;
};
/**
 *@class MtFooter
 *Подпись, может содержаться в MtTemplate, MtHeader.
*/
class MtFooter:public MtTemplateItem
{
   public:
        MtFooter(MtTemplateItem * holder);
        int type()const;
};



#endif // MTTEMPLATE_H
