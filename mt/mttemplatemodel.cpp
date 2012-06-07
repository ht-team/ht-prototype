#include "mttemplatemodel.h"
#include "mttemplate.h"
#include "mtdatamanipulation.h"
#include <QDebug>
#include <QColor>
MtTemplateModel::MtTemplateModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

Qt::ItemFlags MtTemplateModel::flags(const QModelIndex &index) const
{
    if(index.isValid())
    {
        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        MtTemplateItem * item= itemFromIndex(index);
        if(item)
        {
            if(index.column() < item->itemData().size())
            {
                if(!item->itemData().at(index.column())->isReadOnly())
                {
                    flags |= Qt::ItemIsEditable;

                }

            }

        }
        return flags;
    }
    return 0;
}

int MtTemplateModel::rowCount(const QModelIndex &parent) const
{
    const MtTemplateItem * parentItem = itemFromIndex(parent);
    if(!parentItem) return 0;
    return parentItem->childCount();
}

int MtTemplateModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_template->columnCount();
}

QVariant MtTemplateModel::data(const QModelIndex &index, int role) const
{
    const MtTemplateItem * item = itemFromIndex(index);
    if(index.column() < item->itemData().size())
    {

        QVariant data = stateData(item->itemData().at(index.column())->state(), role);
        if(data.isNull())
        {
            switch (role)
            {
            case Qt::EditRole:
            case Qt::DisplayRole:
                return item->itemData().at(index.column())->dataView();
                break;
            default:
                break;
            }

        }
        return data;
    }
    return QVariant();
}

bool MtTemplateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch(role)
    {
        case Qt::EditRole:
        {
            MtTemplateItem * item = itemFromIndex(index);
            if(!item)return false;
            if(index.column()>= item->itemData().size())
            {
                return false;
            }
            MtDataItem * data = item->itemData().at(index.column());
            if(data->isReadOnly()) return false;
            QVariantList vlData = (value.type() == QVariant::List) ? value.toList() : QVariantList()<<value;
            data->setData(vlData);
        }
        break;
        default:
        return QAbstractItemModel::setData(index,value,role);

    }
    return true;
}

QModelIndex MtTemplateModel::
index(int row, int column, const QModelIndex &parent) const
{

    if(row < 0 || column < 0) return QModelIndex();
    MtTemplateItem * item = itemFromIndex(parent);
    if(!item ) return QModelIndex();

    if(row >= item->childCount())
    {

        return QModelIndex();
    }
    QModelIndex i =createIndex(row,column,item->child(row));

    return i;

}

QModelIndex MtTemplateModel::parent(const QModelIndex &child) const
{
    MtTemplateItem * childItem = itemFromIndex(child);
    if(!childItem)
    {
        return QModelIndex();
    }
    MtTemplateItem * parentItem = childItem->parent();
    if(!parentItem || parentItem == m_template)
    {
        return QModelIndex();
    }
    MtTemplateItem * superParent = parentItem->parent();
    if(!superParent)
    {
        superParent = m_template;
    }
    return createIndex(superParent->childs().
                       indexOf(parentItem),0,parentItem);

}

QSize MtTemplateModel::span(const QModelIndex &index) const
{
    MtTemplateItem * item = itemFromIndex(index);
    if(item->itemData().count()-1 == index.column() )
    {
        return QSize(columnCount() - index.column(), 1);
    }
    return QSize();
}

void MtTemplateModel::registerNewState(int state, int role, QVariant value)
{
    m_states[state][role] = value;
}

void MtTemplateModel::registerBackgroundColorState(int state, const QColor &color)
{
    QVariant vColor;
    vColor.setValue(color);
    m_states[state][Qt::BackgroundColorRole] = vColor;
}

void MtTemplateModel::unregisterState(int state, int role)
{
    if(m_states.contains(state))
    {
        m_states[state].remove(role);
        if(!m_states[state].size())
        {
            m_states.remove(state);
        }
    }
}

void MtTemplateModel::unregisterState(int state)
{
    m_states.remove(state);
}

void MtTemplateModel::unregisterAllStates()
{
    m_states.clear();
}

QVariant MtTemplateModel::stateData(int state, int role) const
{
    if(m_states.contains(state))
    {
        if(m_states[state].contains(role))
        {
            return m_states[state][role];
        }
    }
    return QVariant();
}

MtTemplate *MtTemplateModel::handledTemplate()
{
    return m_template;
}


void MtTemplateModel::setHandledTemplate(MtTemplate *pTemplate)
{
    m_template = pTemplate;
    this->reset();
}

MtTemplateItem *MtTemplateModel::itemFromIndex(QModelIndex i) const
{

    if(!i.isValid() )
    {
        return m_template;
    }

    if( i.model() != this)
    {
        return 0;
    }
    return static_cast<MtTemplateItem*>(i.internalPointer());
}

QModelIndex MtTemplateModel::findItem(MtTemplateItem *item, QModelIndex parent)
{
    QModelIndex index;
    QModelIndex tryToFind;
    MtTemplateItem * parentItem = itemFromIndex(parent);
    if(!parentItem || item == m_template|| !item) return index;
    foreach(MtTemplateItem * i , parentItem->childs())
    {
        tryToFind = this->index(parentItem->childs().indexOf(i),0,parent);
        if(i == item)
        {
            return tryToFind;
        }

         tryToFind = findItem(item, tryToFind);
         if(tryToFind.isValid())
         {
             return tryToFind;
         }

    }
    return QModelIndex();

}

void MtTemplateModel::updateItem(MtTemplateItem *item)
{

    QModelIndex i = findItem(item);
    if(!i.isValid() && item != m_template)
    {
        return;
    }

}

void MtTemplateModel::updateItem(QModelIndex index)
{
    MtTemplateItem * item = itemFromIndex(index);
    if(!item) return;
    if(item != m_template)
    {

        emit dataChanged(this->index(index.row(),0,index.parent()),
                         this->index(index.row(),this->columnCount()-1,index.parent()));

    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void MtTemplateModel::update()
{
    reset();
}
