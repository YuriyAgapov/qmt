#pragma once

#include "qmt_entity_model.h"

#include <core/qmt_itable.h>
#include <core/qmt_itable_listener.h>

namespace Qmt
{
    class TableModel :
            public EntityModel,
            public ITableListener,
            public std::enable_shared_from_this<TableModel>
    {
        using ITableSP = std::shared_ptr<ITable>;
    public:
        explicit TableModel(QObject* parent = nullptr);

        QModelIndex index(int row, int column, const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex& child) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        int propertyIndex(const QModelIndex& index) const override;

        ITableSP table() const;
        void setTable(ITableSP table);

    private:
        void aboutChanging(IEntity& entity) override;
        void changed(IEntity& entity) override;
        void rowsAboutToBeInserted(ITable& table, const size_t start, const size_t last) override;
        void rowsInserted(ITable& table, const size_t start, const size_t last) override;
        void rowsAboutToBeRemoved(ITable& table, const size_t start, const size_t last) override;
        void rowsRemoved(ITable& table, const size_t start, const size_t last) override;

        ITableSP _table;
    };

} // namespace Qmt

