#include "mtdatamanipulation.h"
class MtIndicatorConnection
{
public:
    MtIndicatorConnection();
    MtIndicatorConnection(MtDataItem* sourceItem, MtIndicatorItem* indicator = 0,
                          MtCompare* comparer = 0);
    virtual ~MtIndicatorConnection();
    MtIndicatorItem* indicator();
    void setIndicator(MtIndicatorItem* indicator);
    MtDataItem* sourceItem();
    void setSourceItem(MtDataItem* sourceItem);
    MtCompare* comparer();
    void setComparer(MtCompare* comparer);
    bool isConnected() const;
    void disconnect();
private:
    MtIndicatorItem* m_indicator;
    MtDataItem* m_sourceItem;
    MtCompare* m_comparer;
};

MtDataItem::MtDataItem(MtTemplateItem * parent):m_parent(parent)
{

}

 MtDataItem::~MtDataItem()
{

}

const QVariantList &MtDataItem::data() const
{
        return m_data;
}

void MtDataItem::setData(const QVariantList &data)
{
    m_data=data;
}

QVariant MtDataItem::dataView() const
{
    if(m_data.count()>0)return m_data[0];
    else return QVariant();
}

bool MtDataItem::isReadOnly() const
{
    return false;
}

bool MtDataItem::isIndicator() const
{
    return false;
}


const MtTemplateItem *MtDataItem::parent() const
{
    return m_parent;
}

MtDataItem::MtIndicatorItems MtDataItem::indicators() const
{
    return m_indicators;
}


MtIndicatorItem::MtIndicatorItem(MtTemplateItem *parent):MtDataItem(parent),
    m_connection(0)
{
    addWatcher(watch);
}


bool MtIndicatorItem::isIndicator() const
{
    return true;
}

bool MtIndicatorItem::isReadOnly() const
{
    return true;
}

MtTemplateSerialization::MtTemplateSerialization(QObject *parent):QObject(parent)
{
}

void MtDataItem::save()
{
}

void MtDataItem::load()
{
}


MtReadOnlyItem::MtReadOnlyItem(MtTemplateItem *parent)
    :MtDataItem(parent)
{
}

bool MtReadOnlyItem::isReadOnly() const
{
    return true;
}

MtIndicatorConnection::MtIndicatorConnection():m_indicator(0),
    m_sourceItem(0),m_comparer(0)
{
}

MtIndicatorConnection::MtIndicatorConnection(MtDataItem *sourceItem,
                                             MtIndicatorItem *indicator,
                                             MtCompare *comparer):
    m_indicator(indicator), m_sourceItem(sourceItem), m_comparer(comparer)
{
}

MtIndicatorConnection::~MtIndicatorConnection()
{
    disconnect();
}

MtIndicatorItem *MtIndicatorConnection::indicator()
{
    return m_indicator;
}

void MtIndicatorConnection::setIndicator(MtIndicatorItem *indicator)
{
    m_indicator = indicator;
}

MtDataItem *MtIndicatorConnection::sourceItem()
{
    return m_sourceItem;
}

void MtIndicatorConnection::setSourceItem(MtDataItem *sourceItem)
{
    m_sourceItem = sourceItem;
}

MtCompare *MtIndicatorConnection::comparer()
{
    return m_comparer;
}

void MtIndicatorConnection::setComparer(MtCompare *comparer)
{
    m_comparer = comparer;
}

bool MtIndicatorConnection::isConnected() const
{
    return m_indicator && m_sourceItem && m_comparer;
}

void MtIndicatorConnection::disconnect()
{
    m_sourceItem->disconnectIndicator(m_indicator);
    m_indicator->disconnect();
    m_comparer = 0;
}
