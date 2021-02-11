#include "qmt_string_delegate.h"

#include <QCompleter>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItem>

namespace Qmt
{
    void StringDelegateDisplay::paint(QPainter* painter,
                                      const QStyleOptionViewItem& option,
                                      DelegateSettingsCSP settings,
                                      const QVariant& data) const
    {
        const QString text = displayText(option, settings, data);
        painter->setPen(option.palette.color(data.toString().isEmpty() ? QPalette::Disabled
                                                                       : QPalette::Active, QPalette::Text));
        painter->drawText(option.rect, option.displayAlignment, elidedText(option, text));
    }

    QString StringDelegateDisplay::displayText(const QStyleOptionViewItem& option,
                                               DelegateSettingsCSP settings,
                                               const QVariant& data) const
    {
        Q_UNUSED(option);
        auto stringSettings = std::static_pointer_cast<const StringDelegateSettings>(settings);
        QString valueText = data.toString();
        if (valueText.isEmpty())
            valueText = stringSettings->placeholderText;
        return valueText;
    }



    QWidget* StringDelegateEditor::createEditor(QWidget* parent,
                                                const QStyleOptionViewItem& option,
                                                DelegateSettingsCSP settings,
                                                const QVariant& data) const
    {
        auto stringSettings = std::static_pointer_cast<const StringDelegateSettings>(settings);
        QLineEdit* lineEdit = new QLineEdit(parent);
        lineEdit->setText(data.toString());
        lineEdit->setInputMask(stringSettings->inputMask);
        lineEdit->setMaxLength(stringSettings->maxLength);
        if (stringSettings->passwordMode)
            lineEdit->setEchoMode(QLineEdit::Password);
        lineEdit->setValidator(stringSettings->validator);
        lineEdit->setPlaceholderText(stringSettings->placeholderText);
        lineEdit->setGeometry(option.rect);
        lineEdit->setCompleter(stringSettings->completer);
        return lineEdit;
    }

    void StringDelegateEditor::setEditorData(QWidget* editor, DelegateSettingsCSP settings, const QVariant& data) const
    {
        Q_UNUSED(settings);
        static_cast<QLineEdit*>(editor)->setText(data.toString());
    }

    QVariant StringDelegateEditor::editorData(QWidget* editor, DelegateSettingsCSP settings) const
    {
        Q_UNUSED(settings);
        return static_cast<QLineEdit*>(editor)->text();
    }
}//namespace Qmt
