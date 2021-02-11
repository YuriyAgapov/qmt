#include "qmt_number_delegate.h"

#include <QDoubleSpinBox>
#include <QPainter>
#include <QStyleOptionViewItem>

namespace Qmt
{
    QString NumberDelegateDisplay::displayText(const QStyleOptionViewItem& option,
                                               DelegateSettingsCSP settings,
                                               const QVariant& data) const
    {
        Q_UNUSED(option);
        auto numberSettings = std::static_pointer_cast<const NumberDelegateSettings>(settings);

        const double value = data.toDouble();
        const QString valueText = QString("%1%2%3")
                .arg(numberSettings->prefix)
                .arg(value, numberSettings->fieldWidth, 'f', numberSettings->precision)
                .arg(numberSettings->suffix);

        return valueText;
    }



    QWidget* NumberDelegateEditor::createEditor(QWidget* parent,
                                                const QStyleOptionViewItem& option,
                                                DelegateSettingsCSP settings,
                                                const QVariant& data) const
    {
        auto numberSettings = std::static_pointer_cast<const NumberDelegateSettings>(settings);
        QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(parent);
        doubleSpinBox->setValue(data.toDouble());
        if (!std::isnan(numberSettings->min))
            doubleSpinBox->setMinimum(numberSettings->min);
        if (!std::isnan(numberSettings->max))
            doubleSpinBox->setMaximum(numberSettings->max);
        doubleSpinBox->setPrefix(numberSettings->prefix);
        doubleSpinBox->setSuffix(numberSettings->suffix);
        doubleSpinBox->setDecimals(numberSettings->precision);
        doubleSpinBox->setSingleStep(numberSettings->step);
        doubleSpinBox->setGeometry(option.rect);
        return doubleSpinBox;
    }

    void NumberDelegateEditor::setEditorData(QWidget* editor, DelegateSettingsCSP settings, const QVariant& data) const
    {
        Q_UNUSED(settings);
        static_cast<QDoubleSpinBox*>(editor)->setValue(data.toDouble());
    }

    QVariant NumberDelegateEditor::editorData(QWidget* editor, DelegateSettingsCSP settings) const
    {
        Q_UNUSED(settings);
        return static_cast<QDoubleSpinBox*>(editor)->value();
    }
}//namespace Qmt
