#pragma once

#include <QString>
#include <QVariant>

namespace Qmt
{
    class IProperty
    {
    public:
        virtual ~IProperty() = default;

        virtual int typeId() const = 0;
        virtual QString typeName() const = 0;
        virtual QString name() const = 0;
        virtual QVariant defaultValue() const = 0;
    };
}//namespace Qmt
