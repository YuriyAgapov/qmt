#pragma once

#include <core/qmt_itable.h>
#include <models/qmt_table_model.h>

#include <QWidget>

#include <memory>

class QAbstractItemDelegate;
class QItemSelectionModel;
class QToolButton;
class QTreeView;
class QLineEdit;
class QSortFilterProxyModel;

namespace Qmt
{
    class TableModel;

    class TableEditWidget : public QWidget
    {
    public:
        using ITableSP = std::shared_ptr<ITable>;
        using TableModelSP = std::shared_ptr<TableModel>;

        TableEditWidget();

        ITableSP table() const;
        void setTable(ITableSP table);

        QItemSelectionModel* selectionModel() const;

        void setItemDelegate(QAbstractItemDelegate* itemDelegate);
        void setProperties(const EntityModel::NameVector& properties);

    private:
        ITableSP _table;
        TableModelSP _model;
        QSortFilterProxyModel* _proxyModel;
        QLineEdit* _filterEdit;
        QTreeView* _treeView;
        QToolButton* _addButton;
        QToolButton* _removeButton;
        QToolButton* _cloneButton;
    };
}//namespace Qmt
