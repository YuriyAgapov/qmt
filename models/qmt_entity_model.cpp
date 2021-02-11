#include "qmt_entity_model.h"

#include "qmt_idecoration_model.h"
#include "qmt_simple_decoration_model.h"

#include <core/qmt_ientity.h>
#include <core/qmt_iproperty.h>
#include <core/qmt_itype.h>

namespace Qmt
{
    EntityModel::EntityModel(QObject* parent)
        : QAbstractItemModel(parent)
        , _decorationModel(std::make_shared<SimpleDecorationModel>())
    {
    }

    EntityModel::~EntityModel()
    {
    }

    Qt::ItemFlags EntityModel::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        const int typeId = indexType(index);

        if (typeId == QMetaType::Bool)
            flags |= Qt::ItemIsUserCheckable;

        if (QVariant(typeId, nullptr).canConvert(qMetaTypeId<QString>()))
            flags |= Qt::ItemIsEditable;

        return flags;

    }

    const EntityModel::NameVector& EntityModel::properties() const
    {
        return _properties;
    }

    void EntityModel::setProperties(const QStringList& properties)
    {
        _properties.clear();
        _properties.reserve(properties.size());
        for (const auto& name : properties)
            _properties.push_back(std::make_pair(name, name));
        updateTypes();
    }

    void EntityModel::setProperties(const NameVector& properties)
    {
        _properties = properties;
        updateTypes();
    }

    QString EntityModel::propertyName(const int index) const
    {
        return index < 0 ? QString()
                         : _properties.at(index).first;
    }

    QString EntityModel::propertyDisplayName(const int index) const
    {
        return index < 0 ? QString()
                         : _properties.at(index).second;
    }

    QString EntityModel::propertyName(const QModelIndex& modelIndex) const
    {
        return propertyName(propertyIndex(modelIndex));
    }

    int EntityModel::indexType(const QModelIndex& modelIndex) const
    {
        const int index = propertyIndex(modelIndex);
        return index < 0 ? QMetaType::UnknownType
                         : _types.at(index);
    }

    int EntityModel::propertyCount() const
    {
        return static_cast<int>(_properties.size());
    }

    int EntityModel::propertyIndex(const QString& name) const
    {
        for (size_t i = 0; i < _properties.size(); ++i)
        {
            if (_properties[i].first == name)
                return static_cast<int>(i);
        }
        return -1;
    }

    void EntityModel::setEntity(IEntitySP entity)
    {
        _entity = entity;
        if (_properties.empty() && _entity && _entity->type())
        {
            setProperties(_entity->type()->properties());
        }
        else
        {
            updateTypes();
        }
    }

    bool EntityModel::toView(QVariant& value, const QModelIndex& modelIndex, int role) const
    {
        const int typeId = indexType(modelIndex);
        switch (role)
        {
        case Qt::CheckStateRole:
            if (typeId != QMetaType::Bool)
                return false;
            value = value.toBool()
                    ? Qt::Checked
                    : Qt::Unchecked;
            return true;

        case Qt::DisplayRole:
        case Qt::EditRole:
            return typeId != QMetaType::Bool;

        default:
            if (_decorationModel && _decorationModel->toView(value, role, modelIndex, *this))
                return true;
            break;
        }
        return false;
    }

    bool EntityModel::fromView(QVariant& value, const QModelIndex& modelIndex, int role) const
    {
        switch (role)
        {
        case Qt::CheckStateRole:
            if (indexType(modelIndex) != QMetaType::Bool)
                return false;
            value = value.toInt() == Qt::Checked;
            return true;

        case Qt::DisplayRole:
        case Qt::EditRole:
            return true;
        }
        return false;
    }

    void EntityModel::updateTypes()
    {
        _types.clear();
        if (!_entity || !_entity->type())
            return;

        _types.reserve(_properties.size());
        for (const auto& pair : _properties)
        {
            const auto propertyName = pair.first;
            auto property = _entity->type()->property(propertyName);
            if (!property)
            {
                qFatal("%s (%s): property '%s' not found", __FILE__, __func__,
                       qPrintable(propertyName));
            }
            _types.push_back(property->typeId());
        }
    }

    EntityModel::IDecorationModelSP EntityModel::decorationModel() const
    {
        return _decorationModel;
    }

    void EntityModel::setDecorationModel(IDecorationModelSP decorationModel)
    {
        const bool reset = _decorationModel != nullptr;
        if (reset)
            beginResetModel();
        _decorationModel = decorationModel;
        if (reset)
            endResetModel();
    }

}//namespace Qmt
