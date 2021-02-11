#include "qmt_datetime_delegate.h"

#include <QDateTime>
#include <QDateTimeEdit>
#include <QApplication>
#include <QPainter>

namespace Qmt
{
    QString DateTimeDelegateDisplay::displayText(const QStyleOptionViewItem& option,
                                                 DelegateSettingsCSP settings,
                                                 const QVariant& data) const
    {
        Q_UNUSED(option);
        auto dateTimeSettings = std::static_pointer_cast<const DateTimeDelegateSettings>(settings);

        const QDateTime value = data.toDateTime();
        return dateTimeSettings->format.isEmpty() ? value.toString()
                                                  : value.toString(dateTimeSettings->format);

    }

    QWidget* DateTimeDelegateEditor::createEditor(QWidget* parent,
                                                const QStyleOptionViewItem& option,
                                                DelegateSettingsCSP settings,
                                                const QVariant& data) const
    {
        auto dateTimeSettings = std::static_pointer_cast<const DateTimeDelegateSettings>(settings);

        QDateTimeEdit* dateTimeEdit = new QDateTimeEdit(parent);
        dateTimeEdit->setDateTime(data.toDateTime());
        dateTimeEdit->setMaximumDateTime(dateTimeSettings->max);
        dateTimeEdit->setMinimumDateTime(dateTimeSettings->min);
        if (!dateTimeSettings->format.isEmpty())
            dateTimeEdit->setDisplayFormat(dateTimeSettings->format);
        dateTimeEdit->setCalendarPopup(true);
        dateTimeEdit->setGeometry(option.rect);
        return dateTimeEdit;
    }

    void DateTimeDelegateEditor::setEditorData(QWidget* editor,
                                               DelegateSettingsCSP settings,
                                               const QVariant& data) const
    {
        Q_UNUSED(settings);
        static_cast<QDateTimeEdit*>(editor)->setDateTime(data.toDateTime());
    }

    QVariant DateTimeDelegateEditor::editorData(QWidget* editor, DelegateSettingsCSP settings) const
    {
        Q_UNUSED(settings);
        return static_cast<QDateTimeEdit*>(editor)->dateTime();
    }
}//namespace Qmt
