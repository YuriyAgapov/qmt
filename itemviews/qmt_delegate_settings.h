#pragma once

#include <QString>

#include <memory>

namespace Qmt
{
    struct DelegateSettings
    {
        DelegateSettings(const QString& type);
        DelegateSettings(const QString& viewType, const QString& editorType);

        QString viewType;
        QString editorType;
    };

    using DelegateSettingsCSP = std::shared_ptr<const DelegateSettings>;

    inline DelegateSettings::DelegateSettings(const QString& type)
        : viewType(type), editorType(type) {}

    inline DelegateSettings::DelegateSettings(const QString& viewType, const QString& editorType)
        : viewType(viewType), editorType(editorType) {}
}
