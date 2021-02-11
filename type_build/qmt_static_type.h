#pragma once

#include "qmt_type_build_ptr.h"

#include <core/qmt_itype.h>

#include <QHash>

namespace Qmt
{
    class IProperty;

    class StaticType : public IType
    {
    public:
        using PropertyHash = QHash<QString, IPropertyWrapperCSP>;

        StaticType(const int typeId, const PropertyHash&& properties);

        int typeId() const override;
        QString name() const override;

        IPropertyCSP property(const QString& name) const override;
        IPropertyWrapperCSP propertyWrapper(const QString& name) const;
        QStringList properties() const override;

        bool isValid() const override;
        bool operator == (const IType& other) const override;

        bool fromJson(QVariant& value, const QJsonValue& json) const override;
        QJsonValue toJson(const QVariant& value) const override;

    private:
        /// qMetaTypeId of target type
        const int _typeId;
        const PropertyHash _properties;
    };
}//namespace Qmt
