#pragma once

#include "qmt_delegate_settings.h"

#include <QFontMetrics>
#include <QSize>
#include <QStyleOptionViewItem>

class QPainter;
class QVariant;

namespace Qmt
{
    class DelegateDisplay
    {
    public:
        virtual void paint(QPainter* painter,
                           const QStyleOptionViewItem& option,
                           DelegateSettingsCSP settings,
                           const QVariant& data) const;
        virtual QString displayText(const QStyleOptionViewItem& option,
                                    DelegateSettingsCSP settings,
                                    const QVariant& data) const = 0;

        static QString elidedText(const QStyleOptionViewItem& option, const QString& text);
    };
}//namespace Qmt
