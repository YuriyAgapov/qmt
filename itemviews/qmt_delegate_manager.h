#pragma once

#include "qmt_delegate_display.h"
#include "qmt_delegate_editor.h"
#include "qmt_delegate_settings.h"

#include <QHash>
#include <QSize>

#include <memory>

namespace Qmt
{
    class DelegateManager
    {
    public:
        using DelegateDisplaySP = std::shared_ptr<DelegateDisplay>;
        using DelegateEditorSP = std::shared_ptr<DelegateEditor>;

        DelegateManager();

        bool paint(QPainter* painter,
                   const QStyleOptionViewItem& option,
                   DelegateSettingsCSP settings,
                   const QVariant& data) const;

        QString displayText(const QStyleOptionViewItem& option,
                            DelegateSettingsCSP settings,
                            const QVariant& data) const;

        QWidget* createEditor(QWidget* parent,
                              const QStyleOptionViewItem& option,
                              DelegateSettingsCSP settings,
                              const QVariant& data) const;
        void setEditorData(QWidget* editor, DelegateSettingsCSP settings, const QVariant& data) const;
        QVariant editorData(QWidget* editor, DelegateSettingsCSP settings) const;

        void addDisplay(const QString& viewType, DelegateDisplaySP display);
        void addEditor(const QString& editorType, DelegateEditorSP editor);

        static std::shared_ptr<DelegateManager> defaultInstance();

    private:
        QHash<QString, DelegateDisplaySP> _displays;
        QHash<QString, DelegateEditorSP> _editors;
    };
}//namespace Qmt
