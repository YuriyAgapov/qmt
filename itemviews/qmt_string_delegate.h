#pragma once

#include "qmt_delegate_display.h"
#include "qmt_delegate_editor.h"
#include "qmt_delegate_settings.h"

class QCompleter;

namespace Qmt
{
    struct StringDelegateSettings : public DelegateSettings
    {
        StringDelegateSettings() : DelegateSettings(typeName()) {}

        static const char* typeName() { return QMetaType::typeName(QMetaType::QString); }

        QString placeholderText;
        QString inputMask;
        int maxLength = 32767;
        bool passwordMode= false;
        QValidator* validator = nullptr;
        QCompleter* completer = nullptr;
    };

    class StringDelegateDisplay : public DelegateDisplay
    {
    public:
        void paint(QPainter* painter, const QStyleOptionViewItem& option,
                   DelegateSettingsCSP settings,
                   const QVariant& data) const;
        QString displayText(const QStyleOptionViewItem& option,
                            DelegateSettingsCSP settings,
                            const QVariant& data) const override;
    };

    class StringDelegateEditor : public DelegateEditor
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
