#include "qmt_table_model.h"

#include <core/qmt_ientity.h>

namespace Qmt
{

    TableModel::TableModel(QObject* parent) : EntityModel(parent)
    {
    }

    QModelIndex TableModel::index(int row, int column, const QModelIndex& parent) const
    {
        if (!_table || parent.isValid())
            return QModelIndex();
        return createIndex(row, column);
    }

    QModelIndex TableModel::parent(const QModelIndex& child) const
    {
        Q_UNUSED(child);
        return QModelIndex();
    }

    int TableModel::rowCount(const QModelIndex& parent) const
    {
        if (!_table || parent.isValid())
            return 0;
        return static_cast<int>(_table->size());
    }

    int TableModel::columnCount(const QModelIndex& parent) const
    {
        if (!_table || parent.isValid())
            return 0;
        return propertyCount();
    }

    QVariant TableModel::data(const QModelIndex& index, int role) const
    {
        if (!_table || index.parent().isValid())
            return QVariant();

        const QString name = propertyName(index);
        QVariant value = _table->property(name, index.row());
        return toView(value, index, role) ? value : QVariant();
    }

    bool TableModel::setData(const QModelIndex& index, const QVariant& data, int role)
    {
        if (!_table || index.parent().isValid())
            return false;

        const QString name = propertyName(index);
        QVariant value = data;
        if (fromView(value, index, role))
        {
            _table->setProperty(name, index.row(), value);
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (!_table || orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        return propertyDisplayName(section);
    }

    int TableModel::propertyIndex(const QModelIndex& index) const
    {
        return index.column();
    }

    TableModel::ITableSP TableModel::table() const
    {
        return _table;
    }

    void TableModel::setTable(ITableSP table)
    {
        if (_table == table)
            return;

        beginResetModel();

        if (_table)
            _table->entity()->removeListener(shared_from_this());

        _table = table;

        if (_table)
        {
            setEntity(_table->entity());
            _table->entity()->addListener(shared_from_this());
        }

        endResetModel();
    }

    void TableModel::aboutChanging(IEntity& entity)
    {
        Q_UNUSED(entity);

        beginResetModel();
    }

    void TableModel::changed(IEntity& entity)
    {
        Q_UNUSED(entity);

        endResetModel();
    }

    void TableModel::rowsAboutToBeInserted(ITable& table, const size_t start, const size_t last)
    {
        Q_UNUSED(table);

        beginInsertRows(QModelIndex(), static_cast<int>(start), static_cast<int>(last));
    }

    void TableModel::rowsInserted(ITable& table, const size_t start, const size_t last)
    {
        Q_UNUSED(table);
        Q_UNUSED(start);
        Q_UNUSED(last);

        endInsertRows();
    }

    void TableModel::rowsAboutToBeRemoved(ITable& table, const size_t start, const size_t last)
    {
        Q_UNUSED(table);

        beginRemoveRows(QModelIndex(), static_cast<int>(start), static_cast<int>(last));
    }

    void TableModel::rowsRemoved(ITable& table, const size_t start, const size_t last)
    {
        Q_UNUSED(table);
        Q_UNUSED(start);
        Q_UNUSED(last);

        endRemoveRows();
    }
} // namespace Qmt
