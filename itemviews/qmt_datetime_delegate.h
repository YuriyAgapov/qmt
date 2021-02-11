#pragma once

#include "qmt_delegate_display.h"
#include "qmt_delegate_editor.h"
#include "qmt_delegate_settings.h"

#include <QDateTime>

namespace Qmt
{
    struct DateTimeDelegateSettings : public DelegateSettings
    {
        DateTimeDelegateSettings() : DelegateSettings(typeName()) {}

        static const char* typeName() { return QMetaType::typeName(QMetaType::QDateTime); }

        QDateTime min;
        QDateTime max;
        QString format;
    };

    class DateTimeDelegateDisplay : public DelegateDisplay
    {
    public:
        QString displayText(const QStyleOptionViewItem& option,
                            DelegateSettingsCSP settings,
                            const QVariant& data) const override;
    };

    class DateTimeDelegateEditor : public DelegateEditor
    {
    public:
        QWidget* createEditor(QWidget* parent,
                              const QStyleOptionViewItem& option,
                              DelegateSettingsCSP settings,
                              const QVariant& data) const;
        void setEditorData(QWidget* editor,
                           DelegateSettingsCSP settings,
                           const QVariant& data) const;
        QVariant editorData(QWidget* editor, DelegateSettingsCSP settings) const;
    };
}//namespace Qmt
