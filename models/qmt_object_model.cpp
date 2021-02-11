#include "qmt_object_model.h"

#include <core/qmt_ientity.h>

namespace Qmt
{

    ObjectModel::ObjectModel(QObject* parent)
        : EntityModel(parent)
    {
    }

    QModelIndex ObjectModel::index(int row, int column, const QModelIndex& parent) const
    {
        if (parent.isValid())
            return QModelIndex();
        return createIndex(row, column);
    }

    QModelIndex ObjectModel::parent(const QModelIndex& child) const
    {
        Q_UNUSED(child);
        return QModelIndex();
    }

    int ObjectModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return propertyCount();
    }

    int ObjectModel::columnCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent);
        return 2;//ColumnName and ColumnValue
    }

    QVariant ObjectModel::data(const QModelIndex& modelIndex, int role) const
    {
        if (modelIndex.parent().isValid())
            return QVariant();

        const int propertyId = propertyIndex(modelIndex);
        if (propertyId < 0)
            return QVariant();

        const auto name = propertyName(propertyId);

        switch (modelIndex.column())
        {
        case ColumnName:
            if (role != Qt::EditRole && role != Qt::DisplayRole)
                return QVariant();
            return propertyDisplayName(propertyId);

        case ColumnValue:
            {
                QVariant value = _object->property(name);
                if (toView(value, modelIndex, role))
                    return value;
                return QVariant();
            }
        }
        return QVariant();
    }

    bool ObjectModel::setData(const QModelIndex& index, const QVariant& data, int role)
    {
        if (index.parent().isValid() || index.column() != ColumnValue)
            return false;

        const QString name = propertyName(index);

        QVariant value = data;
        return fromView(value, index, role)
            && _object->setProperty(name, value);
    }

    Qt::ItemFlags ObjectModel::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        Qt::ItemFlags flags = QAbstractItemModel::flags(index);

        if (index.column() == ColumnValue)
        {
            flags |= EntityModel::flags(index);
        }
        return static_cast<Qt::ItemFlags>(flags);
    }

    QVariant ObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        switch (section)
        {
        case ColumnName: return tr("Name");
        case ColumnValue: return tr("Value");
        }
        return QVariant();
    }

    int ObjectModel::indexType(const QModelIndex& modelIndex) const
    {
        switch (modelIndex.column())
        {
        case ColumnName: return QMetaType::QString;
        case ColumnValue: return EntityModel::indexType(modelIndex);
        }
        return QMetaType::UnknownType;
    }

    int ObjectModel::propertyIndex(const QModelIndex& index) const
    {
        return index.row();
    }

    ObjectModel::IObjectSP ObjectModel::object() const
    {
        return _object;
    }

    void ObjectModel::setObject(IObjectSP object)
    {
        if (_object == object)
            return;

        beginResetModel();
        if (_object)
            _object->entity()->removeListener(shared_from_this());

        _object = object;
        if (_object)
        {
            setEntity(_object->entity());
            _object->entity()->addListener(shared_from_this());
        }
        endResetModel();
    }

    void ObjectModel::aboutChanging(IEntity& entity)
    {
        Q_UNUSED(entity);
        beginResetModel();
    }

    void ObjectModel::changed(IEntity& entity)
    {
        Q_UNUSED(entity);
        endResetModel();
    }

    void ObjectModel::propertyChanged(IObject& object, const QString& name, const QVariant& value)
    {
        Q_UNUSED(object);
        Q_UNUSED(value);
        const int row = propertyIndex(name);
        if (row < 0)
            return;

        const QModelIndex modelIndex = index(row, ColumnValue, QModelIndex());
        emit dataChanged(modelIndex, modelIndex);
    }
} // namespace Qmt
