#ifndef MTTEMPLATEMODEL_H
#define MTTEMPLATEMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QVariant>
class MtTemplate;
class MtTemplateItem;
class MtTemplateModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MtTemplateModel(QObject *parent = 0);

    MtTemplate * handledTemplate();
    void setHandledTemplate(MtTemplate * pTemplate);

    MtTemplateItem *itemFromIndex(QModelIndex i) const;
    QModelIndex findItem(MtTemplateItem * item, QModelIndex parent = QModelIndex());
    void updateItem(MtTemplateItem * item);
    void updateItem(QModelIndex item);
    void update();

    //reimplemented:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(int row,
                      int column,
                      const QModelIndex &parent =
            QModelIndex()) const;
    QModelIndex  parent(const QModelIndex &child) const;
    QSize span(const QModelIndex & index) const;

    //state based functions
    void registerNewState(int state, int role, QVariant value);
    void registerBackgroundColorState(int state, const QColor& color);

    void unregisterState(int state, int role);
    void unregisterState(int state);

    void unregisterAllStates();


public slots:
private:
    QVariant stateData(int state, int role) const;
    typedef QMap<int, QVariant> RolePredifinedDataMapping;
    typedef QMap<int, RolePredifinedDataMapping > States;
    States m_states;
    MtTemplate * m_template;
};

#endif // MTTEMPLATEMODEL_H
