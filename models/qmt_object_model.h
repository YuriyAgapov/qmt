#pragma once

#include "qmt_entity_model.h"

#include <core/qmt_iobject.h>
#include <core/qmt_iobject_listener.h>

#include <QVariant>

namespace Qmt
{
    class IPropertyRepresentation;

    class ObjectModel :
            public EntityModel,
            public IObjectListener,
            public std::enable_shared_from_this<ObjectModel>
    {
        using IObjectSP = std::shared_ptr<IObject>;
    public:
        enum Column
        {
            ColumnName = 0,
            ColumnValue = 1
        };

        explicit ObjectModel(QObject* parent = nullptr);

        QModelIndex index(int row, int column, const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex& child) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        int indexType(const QModelIndex& modelIndex) const override;
        using EntityModel::propertyIndex;
        int propertyIndex(const QModelIndex& index) const override;

        IObjectSP object() const;
        void setObject(IObjectSP object);

    private:
        void aboutChanging(IEntity& entity) override;
        void changed(IEntity& entity) override;
        void propertyChanged(IObject& object, const QString& name, const QVariant& value) override;

        IObjectSP _object;
    };

} // namespace Qmt

