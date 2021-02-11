#pragma once

#include "qmt_delegate_manager.h"
#include "qmt_delegate_settings_model.h"

#include <QItemDelegate>

#include <memory>

namespace Qmt
{
    class ItemDelegate : public QItemDelegate
    {
    public:
        using IDelegateSettingsModelSP = std::shared_ptr<IDelegateSettingsModel>;
        using DelegateManagerSP = std::shared_ptr<DelegateManager>;

        ItemDelegate(DelegateManagerSP manager = nullptr, IDelegateSettingsModelSP settingsModel = nullptr);

        QWidget* createEditor(QWidget* parent,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;
        void paint(QPainter* painter,
                   const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;
        void setEditorData(QWidget* editor, const QModelIndex& index) const override;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

        DelegateManagerSP manager() const;
        void setManager(DelegateManagerSP manager);

        IDelegateSettingsModelSP settingsModel() const;
        void setSettingsModel(IDelegateSettingsModelSP settingsModel);

    private:
        void drawDisplay(QPainter* painter,
                         const QStyleOptionViewItem& option,
                         const QRect& rect,
                         const QString& text) const override;

        mutable QModelIndex _currentIndex;
        DelegateManagerSP _manager;
        IDelegateSettingsModelSP _settingsModel;
    };
}//namespace Qmt
