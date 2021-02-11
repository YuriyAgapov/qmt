#pragma once

#include "qmt_idelegate_settings_model.h"

#include <QHash>

class QModelIndex;

namespace Qmt
{
    class DelegateSettingsModel : public IDelegateSettingsModel
    {
    public:
        DelegateSettingsCSP settings(const QModelIndex &index) const override;
        void setSettings(const QString& property, DelegateSettingsCSP settings);

    private:
        QHash<QString, DelegateSettingsCSP> _settings;
    };
}//namespace Qmt
