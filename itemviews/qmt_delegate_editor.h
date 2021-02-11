#pragma once

#include "qmt_delegate_settings.h"

class QWidget;
class QStyleOptionViewItem;
class QVariant;

namespace Qmt
{
    class DelegateEditor
    {
    public:
        virtual QWidget* createEditor(QWidget* parent,
                                      const QStyleOptionViewItem& option,
                                      DelegateSettingsCSP settings,
                                      const QVariant& data) const = 0;
        virtual void setEditorData(QWidget* editor, DelegateSettingsCSP settings, const QVariant& data) const = 0;
        virtual QVariant editorData(QWidget* editor, DelegateSettingsCSP settings) const = 0;
    };
}//namespace Qmt
