#include "mttemplate.h"
#include "mtdatamanipulation.h"
MtTemplate::MtTemplate(MtTemplateFactory *factory):MtFooterHolder(this), m_factory(factory), m_columns(0)
{
}



MtTemplateItem::MtTemplateItem(MtTemplate * parentTemplate):m_parentTemplate(parentTemplate)
{
    setParent(0);
}

MtTemplateItem::~MtTemplateItem()
{
    foreach(MtTemplateItem * i,childs())
    {
        removeChild(i);
    }
    if(parent())
    {
        parent()->removeChild(this);
    }
    foreach (MtDataItem* data, this->itemData())
    {
        delete data;
    }
}

MtTemplateItem * MtTemplateItem::child(int at)
{
    return childs().at(at);
}

const MtTemplateItem * MtTemplateItem::child(int at) const
{
    return childs().at(at);
}

bool MtTemplateItem::removeChild(MtTemplateItem *child)
{
    foreach (MtTemplateItem* item, childs())
    {
        if(item == child)
        {

            childs().removeAll(child);
            return true;
        }
    }
    return false;
}

bool MtTemplateItem::removeChild(int at)
{
    if(at>=childs().count()||at<0)return false;
    delete childs().at(at);
    childs().removeAt(at);
    return true;
}

bool MtTemplateItem::swapChild(int from, int too)
{
    if(from>=childCount()||from<0||too>=childCount()||too<0||too==from)return false;
    childs().swap(from,too);
    return true;
}

bool MtTemplateItem::swapChild(MtTemplateItem* which,MtTemplateItem * with)
{
    int from=childs().indexOf(which);
    int to=childs().indexOf(with);
    return swapChild(from,to);
}

bool MtTemplateItem::swapChild(MtTemplateItem *which, int to)
{
    int from=childs().indexOf(which);
    return swapChild(from,to);

}

int MtTemplateItem::childCount() const
{
    return childs().count();
}

bool MtTemplateItem::hasChilds() const
{
    return childs().count()>0;
}

MtTemplateItem * MtTemplateItem::parent()
{
    return m_parent;
}

MtTemplate *MtTemplateItem::parentTemplate()
{
    return m_parentTemplate;
}

MtTemplateItem::MtTemplateItems & MtTemplateItem::childs()
{
    return m_childs;
}

const MtTemplateItem::MtTemplateItems & MtTemplateItem::childs() const
{
    return m_childs;
}

MtTemplateItem::ItemData &MtTemplateItem::itemData()
{
    return m_itemData;
}

void MtTemplateItem::setParent(MtTemplateItem *parent)
{
    m_parent=parent;
}
MtTemplate * MtTemplateItem::toTemplate()
{
    if(type()==Template)
        return static_cast<MtTemplate*>(this);
    else
        return 0;
}
MtHeader * MtTemplateItem::toHeader()
{
    if(type()==Header)
        return static_cast<MtHeader*>(this);
    else
        return 0;
}
MtSubHeader * MtTemplateItem::toSubHeader()
{
    if(type()==Subheader)
        return static_cast<MtSubHeader*>(this);
    else
        return 0;
}
MtFooter * MtTemplateItem::toFooter()
{
    if(type()==Footer)
        return static_cast<MtFooter*>(this);
    else
        return 0;
}
MtFooterHolder::MtFooterHolder(MtTemplate * parentTemplate):MtTemplateItem(parentTemplate)
{

}

MtFooterHolder::~MtFooterHolder()
{
    foreach(MtFooter * f,m_footers)
    {
        phyzicalHolder()->removeChild(f);
        delete f;
    }
}

MtFooterHolder::MtFooters & MtFooterHolder::footers()
{
    return m_footers;
}

const MtFooterHolder::MtFooters & MtFooterHolder::footers() const
{
    return m_footers;
}

MtFooter * MtFooterHolder::addFooter()
{
    MtFooter * f = new MtFooter(phyzicalHolder());
    MtTemplate * t = parentTemplate();
    f->itemData()= t->factory()
            ->defaultFooterData(t->templateType(),f);
    footers().append(f);
    int index=phyzicalHolder()->childs().indexOf(this);
    if(index<0)
        phyzicalHolder()->childs().append(f);
    else
        phyzicalHolder()->childs().insert(index+1,f);
    return f;
}



MtTemplateItem * MtFooterHolder::phyzicalHolder()
{
    MtTemplateItem * physicalHolder=this->parent();
    if(!physicalHolder)physicalHolder=this;
    return physicalHolder;
}

int MtTemplate::columnCount() const
{
    return m_columns;
}

void MtTemplate::setColumnCount(int columnCount)
{
    m_columns=columnCount;
}

int MtTemplate::type() const
{
    return Template;
}

MtHeader * MtTemplate::addHeader()
{
    MtHeader * h = new MtHeader(this);
    MtTemplate * t = this;
    h->itemData()= t->factory()
            ->defaultHeaderData(t->templateType(),h);
    this->childs().append(h);
    return h;
}

MtTemplateFactory *MtTemplate::factory()
{
    return m_factory;
}

MtHeader::MtHeader(MtTemplate *parent):MtFooterHolder(parent)
{
    this->setParent(parent);
}

MtHeader::MtHeader(MtHeader * parent):MtFooterHolder(parent->parentTemplate())
{
    setParent(parent);
}

int MtHeader::type() const
{
    return Header;
}

MtHeader * MtHeader::addHeader()
{
    MtHeader * h=new MtHeader(this);
    MtTemplate * t = parentTemplate();
    h->itemData()= t->factory()
            ->defaultHeaderData(t->templateType(),h);
    this->childs().append(h);
    return h;
}

MtSubHeader * MtHeader::addSubHeader()
{
    MtSubHeader*h=new MtSubHeader(this);
    MtTemplate * t = parentTemplate();
    h->itemData()= t->factory()
            ->defaultSubHeaderData(t->templateType(),h);
    this->childs().append(h);
    return h;
}

MtSubHeader::MtSubHeader(MtHeader *parent) :
    MtTemplateItem(parent ? parent->parentTemplate() : 0)
{
    setParent(parent);
}

int MtSubHeader::type() const
{
    return Subheader;
}

MtFooter::MtFooter(MtTemplateItem *holder) :
    MtTemplateItem(holder ? holder->parentTemplate() : 0)
{
    setParent(holder);
}

int MtFooter::type() const
{
    return Footer;
}


const MtTemplateItem::ItemData &MtTemplateItem::itemData() const
{
    return m_itemData;
}


MtTemplateFactory::TemplateType MtTemplate::templateType() const
{
    return m_type;
}

void MtTemplate::setTemplateType(MtTemplateFactory::TemplateType type)
{
    m_type = type;
}
