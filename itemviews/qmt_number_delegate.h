#pragma once

#include "qmt_delegate_display.h"
#include "qmt_delegate_editor.h"
#include "qmt_delegate_settings.h"
#include "qmt_delegate_settings.h"

#include <cmath>

namespace Qmt
{
    struct NumberDelegateSettings : public DelegateSettings
    {
        NumberDelegateSettings() : DelegateSettings(typeName()) {}

        static const char* typeName() { return QMetaType::typeName(QMetaType::Double); }

        double min = -1000;
        double max = 1000;
        double step = 1.0;
        int precision = 0;
        int fieldWidth = 0;
        QString prefix;
        QString suffix;
    };

    class NumberDelegateDisplay : public DelegateDisplay
    {
    public:
        QString displayText(const QStyleOptionViewItem& option,
                            DelegateSettingsCSP settings,
                            const QVariant& data) const override;
    };

    class NumberDelegateEditor : public DelegateEditor
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
