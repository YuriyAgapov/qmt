#include "qmt_delegate_display.h"

#include <QPainter>

namespace Qmt
{
    void DelegateDisplay::paint(QPainter* painter,
                                const QStyleOptionViewItem& option,
                                DelegateSettingsCSP settings,
                                const QVariant& data) const
    {
        painter->drawText(option.rect, option.displayAlignment, elidedText(option, displayText(option, settings, data)));
    }

    QString DelegateDisplay::elidedText(const QStyleOptionViewItem& option, const QString& text)
    {
        return option.fontMetrics.elidedText(text, option.textElideMode, option.rect.width());
    }
}//namespace Qmt
