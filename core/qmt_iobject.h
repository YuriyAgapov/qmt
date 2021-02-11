#pragma once

#include <QString>
#include <QVariant>

#include <memory>

namespace Qmt
{
    class IEntity;

    class IObject
    {
    public:
        using IEntitySP = std::shared_ptr<IEntity>;

        virtual ~IObject() = default;

        virtual IEntitySP entity() = 0;
        virtual QVariant property(const QString& name) const = 0;
        virtual bool setProperty(const QString& name, const QVariant& value) = 0;
    };
}
