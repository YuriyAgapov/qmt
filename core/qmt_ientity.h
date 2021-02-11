#pragma once

#include <core/qmt_ptr.h>

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QVariant>

class QJsonValue;

namespace Qmt
{
    class IEntityListener;
    class IType;

    class IEntity
    {
    public:
        virtual ~IEntity() = default;

        virtual ITypeSP type() const = 0;
        virtual QVariant toVariant() const = 0;
        virtual void fromVariant(const QVariant& value) = 0;
        virtual QJsonValue toJson() const = 0;
        virtual void fromJson(const QJsonValue& value) = 0;
        virtual void debug(QDebug& debug) const = 0;

        virtual void addListener(IEntityListenerWP listener) = 0;
        virtual void removeListener(IEntityListenerWP listener) = 0;
    };

#ifndef Q_NO_DEBUG
    inline QDebug operator << (QDebug debug, const IEntity& entity)
    {
        entity.debug(debug);
        return debug;
    }
#endif//Q_NO_DEBUG
}
