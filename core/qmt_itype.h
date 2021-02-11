#pragma once

#include "qmt_ptr.h"

#include <QString>
#include <QStringList>
#include <QVariant>

namespace Qmt
{
    class IType
    {
    public:
        virtual ~IType() = default;

        virtual int typeId() const = 0;
        virtual QString name() const = 0;

        virtual QStringList properties() const = 0;
        virtual IPropertyCSP property(const QString& name) const = 0;

        virtual bool isValid() const = 0;
        virtual bool operator == (const IType& other) const = 0;

        virtual bool fromJson(QVariant& value, const QJsonValue& json) const = 0;
        virtual QJsonValue toJson(const QVariant& value) const = 0;
    };
}//namespace Qmt
