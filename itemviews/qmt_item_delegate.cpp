#include "qmt_item_delegate.h"

#include <QDebug>

namespace Qmt
{
    ItemDelegate::ItemDelegate(DelegateManagerSP manager,
                               IDelegateSettingsModelSP settingsModel)
        : _manager(manager)
        , _settingsModel(settingsModel)
    {
    }

    QWidget* ItemDelegate::createEditor(QWidget* parent,
                                        const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
    {
        const auto settings = _settingsModel->settings(index);
        return settings ? _manager->createEditor(parent, option, settings, index.data(Qt::EditRole))
                        : QItemDelegate::createEditor(parent, option, index);
    }

    void ItemDelegate::paint(QPainter* painter,
                             const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
    {
        _currentIndex = index;
        QItemDelegate::paint(painter, option, index);
        _currentIndex = QModelIndex();
    }

    QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        return QItemDelegate::sizeHint(option, index);
        QSize size = QItemDelegate::sizeHint(option, index);
        auto settings = _settingsModel->settings(index);
        if (settings)
        {
            const QString displayText = _manager->displayText(option, settings, index.data(Qt::DisplayRole));

            if (!displayText.isEmpty())
            {
                const int width = option.fontMetrics.width(displayText);
                size.setWidth(std::max(size.width(), width + 10));
            }
        }
        return size;
    }

    void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        const auto settings = _settingsModel->settings(index);
        if (settings)
            _manager->setEditorData(editor, settings, index.data(Qt::EditRole));
        else
            QItemDelegate::setEditorData(editor, index);
    }

    void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        const auto settings = _settingsModel->settings(index);
        if (settings)
            model->setData(index, _manager->editorData(editor, settings), Qt::EditRole);
        else
            QItemDelegate::setModelData(editor, model, index);
    }

    ItemDelegate::IDelegateSettingsModelSP ItemDelegate::settingsModel() const
    {
        return _settingsModel;
    }

    void ItemDelegate::setSettingsModel(IDelegateSettingsModelSP settingsModel)
    {
        _settingsModel = settingsModel;
    }

    ItemDelegate::DelegateManagerSP ItemDelegate::manager() const
    {
        return _manager;
    }

    void ItemDelegate::setManager(DelegateManagerSP manager)
    {
        _manager = manager;
    }

    void ItemDelegate::drawDisplay(QPainter* painter,
                                   const QStyleOptionViewItem& option,
                                   const QRect& rect,
                                   const QString& text) const
    {
        QItemDelegate::drawDisplay(painter, option, rect, text);
        return;
        auto settings = _settingsModel->settings(_currentIndex);
        if (settings)
        {
            if (!_manager->paint(painter, option, settings, _currentIndex.data(Qt::DisplayRole)))
                QItemDelegate::drawDisplay(painter, option, rect, text);
        }
        else
        {
            QItemDelegate::drawDisplay(painter, option, rect, text);
        }
    }
}//namespace Qmt
