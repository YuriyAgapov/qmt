#pragma once

#include <core/qmt_iproperty.h>

#include <QJsonValue>
#include <QVariant>

namespace Qmt
{
    class IPropertyWrapper : public IProperty
    {
    public:
        virtual QVariant toVariant(const void* object) const = 0;
        virtual void fromVariant(void* object, const QVariant& value) const = 0;
        virtual bool debug(QDebug& debug, const void* object) const = 0;
        virtual QJsonValue toJsonValue(const void* object) const = 0;
        virtual void fromJsonValue(void* object, const QJsonValue& json) const = 0;
    };
}//namespace Qmt
