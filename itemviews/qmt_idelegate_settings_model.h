#pragma once

#include "qmt_delegate_settings.h"

class QModelIndex;

namespace Qmt
{
    class IDelegateSettingsModel
    {
    public:
        virtual ~IDelegateSettingsModel() = default;

        virtual DelegateSettingsCSP settings(const QModelIndex &index) const = 0;
    };
}//namespace Qmt
