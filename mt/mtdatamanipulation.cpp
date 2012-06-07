#include "mtdatamanipulation.h"
#include <mtcompare.h>
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
     foreach(MtIndicatorConnection* connection, m_connections)
     {
         if(connection)
             connection->disconnect();
         else
             delete connection;
         //TODO: extract method, and make it protected and review connect/dissconnect functions, for feature of make on other side ref for connection null, and review all MtTemplateItem and MtDataItem destructors are safe.
     }
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

int MtDataItem::state() const
{
    return 0;
}


MtTemplateItem *MtDataItem::parent()
{
    return m_parent;
}

MtIndicatorConnection* MtDataItem::connectIndicator(MtIndicatorItem *indicator, MtCompare *comparer)
{
    if(!indicator)
    {
        return 0;
    }
    MtIndicatorConnection* reconnection = 0;
    foreach(MtIndicatorConnection* connection, m_connections)
    {
        if(connection->indicator() == indicator)
        {
            reconnection = connection;
        }
    }

    if(!reconnection)
    {
        reconnection = new MtIndicatorConnection(this, indicator, comparer);
    }
    else
    {
        reconnection->setIndicator(indicator);
        reconnection->setSourceItem(this);
        reconnection->setComparer(comparer);
    }
    return reconnection;
}

void MtDataItem::disconnectIndicator(MtIndicatorItem *indicator)
{
    if(!indicator)
    {
        return;
    }
    MtIndicatorConnection* connection = 0;
    foreach (MtIndicatorConnection* i, m_connections)
    {
        if(i->indicator() == indicator)
        {
            connection = i;
            break;
        }
    }

    if(connection)
    {
        m_connections.remove(connection);
        connection->setIndicator(0);
        delete connection;
    }
}

MtDataItem::MtIndicatorItems MtDataItem::indicators() const
{
    QSet<MtIndicatorItem*> indicators;
    foreach(MtIndicatorConnection* connection, m_connections)
    {
        if(connection->isConnected())
        {
            indicators.insert(connection->indicator());
        }
    }

    return indicators.toList();
}


MtIndicatorItem::MtIndicatorItem(MtTemplateItem *parent):MtDataItem(parent),
    m_connection(0)
{

}

MtIndicatorItem::~MtIndicatorItem()
{
    if(!m_connection)
        return;
    if(m_connection->isConnected())
    {
        m_connection->disconnect();
    }
    delete m_connection;
}


void MtIndicatorItem::disconnect()
{
    if(m_connection)
    {
        m_connection->setIndicator(0);
        m_connection->disconnect();
    }
    m_connection = 0;
}

MtDataItem *MtIndicatorItem::sourceItem()
{
    if(m_connection)
    {
        return m_connection->sourceItem();
    }
    return 0;
}

int MtIndicatorItem::state() const
{
    if(!m_connection || !m_connection->isConnected())
    {
        return MtDataItem::state();
    }
    return m_connection->comparer()->test(m_connection->sourceItem());
}


bool MtIndicatorItem::isIndicator() const
{
    return true;
}

bool MtIndicatorItem::isReadOnly() const
{
    return true;
}

void MtIndicatorItem::setupConnection(MtIndicatorConnection *connection)
{
    if(!m_connection)
    {
        m_connection = connection;
    }
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
    if(indicator)
        indicator->setupConnection(this);
    if(sourceItem)
        sourceItem->setupConnection(this);
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
    m_sourceItem = 0;
    m_indicator = 0;
}

void MtDataItem::disconnectAllIndicators()
{
    MtIndicatorItems indicatorItems = indicators();
    foreach(MtIndicatorItem* indicator, indicatorItems)
    {
        disconnectIndicator(indicator);
    }
}

void MtDataItem::setupConnection(MtIndicatorConnection *connection)
{
    if(!connection)
        return;
    m_connections.insert(connection);
}
