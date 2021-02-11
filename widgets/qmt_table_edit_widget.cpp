#include "qmt_table_edit_widget.h"

#include <core/qmt_ientity.h>
#include <core/qmt_itype.h>

#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSortFilterProxyModel>

namespace Qmt
{
    TableEditWidget::TableEditWidget()
        : _model(std::make_shared<TableModel>())
        , _proxyModel(new QSortFilterProxyModel())
        , _filterEdit(new QLineEdit())
        , _treeView(new QTreeView())
        , _addButton(new QToolButton())
        , _removeButton(new QToolButton())
        , _cloneButton(new QToolButton())
    {
        _proxyModel->setSourceModel(_model.get());
        _treeView->setModel(_proxyModel);
        _treeView->setRootIsDecorated(false);
        _treeView->setAlternatingRowColors(true);
        _treeView->setSelectionBehavior(QTreeView::SelectItems);

        _addButton->setIcon(QIcon::fromTheme("list-add"));
        _addButton->setEnabled(false);
        _removeButton->setIcon(QIcon::fromTheme("list-remove"));
        _removeButton->setEnabled(false);
        _cloneButton->setIcon(QIcon::fromTheme("edit-copy"));
        _cloneButton->setEnabled(false);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(_addButton);
        buttonLayout->addWidget(_removeButton);
        buttonLayout->addWidget(_cloneButton);
        buttonLayout->addStretch(1);
        buttonLayout->setMargin(0);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(_filterEdit);
        mainLayout->addWidget(_treeView);
        mainLayout->addLayout(buttonLayout);
        mainLayout->setMargin(0);

        connect(_addButton, &QToolButton::clicked, this, [this]()
        {
            if (!_table)
                return;
            _table->insert(_table->size());
        });
        connect(_removeButton, &QToolButton::clicked, this, [this]()
        {
            if (!_table)
                return;

            _table->remove(_treeView->currentIndex().row());
        });
        connect(_cloneButton, &QToolButton::clicked, this, [this]()
        {
            if (!_table)
                return;

            const int rowIndex = _treeView->currentIndex().row();
            auto data = _table->rowProperties(rowIndex);
            _table->insert(rowIndex + 1, data);
        });
        connect(_filterEdit, &QLineEdit::textChanged, this, [this](const QString& text)
        {
            _proxyModel->setFilterWildcard(text);
        });
    }

    TableEditWidget::ITableSP TableEditWidget::table() const
    {
        return _table;
    }

    void TableEditWidget::setTable(ITableSP table)
    {
        _table = table;
        _model->setTable(_table);
        _addButton->setEnabled(_table.get());
        _removeButton->setEnabled(_table.get());
        _cloneButton->setEnabled(_table.get());
    }

    QItemSelectionModel* TableEditWidget::selectionModel() const
    {
        return _treeView->selectionModel();
    }

    void TableEditWidget::setItemDelegate(QAbstractItemDelegate* itemDelegate)
    {
        _treeView->setItemDelegate(itemDelegate);
    }

    void TableEditWidget::setProperties(const EntityModel::NameVector& properties)
    {
        _model->setProperties(properties);
    }
}//namespace Qmt
