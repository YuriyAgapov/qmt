#include "qmt_delegate_settings_model.h"

#include <models/qmt_entity_model.h>

namespace Qmt
{
    Qmt::DelegateSettingsCSP DelegateSettingsModel::settings(const QModelIndex& index) const
    {
        const Qmt::EntityModel* entityModel = dynamic_cast<const Qmt::EntityModel*>(index.model());
        if (!entityModel)
            return nullptr;

        if (!(entityModel->flags(index) & Qt::ItemIsEditable))
            return nullptr;

        const QString name = entityModel->propertyName(index);
        return _settings.value(name);
    }

    void DelegateSettingsModel::setSettings(const QString& property, DelegateSettingsCSP settings)
    {
        _settings[property] = settings;
    }
}//namespace Qmt
