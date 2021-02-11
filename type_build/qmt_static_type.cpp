#include "qmt_static_type.h"

#include "qmt_iproperty_wrapper.h"

#include <QMetaType>
#include <QJsonObject>

namespace Qmt
{
    StaticType::StaticType(const int typeId, const PropertyHash&& properties)
        : _typeId(typeId)
        , _properties(properties)
    {
    }

    int StaticType::typeId() const
    {
        return _typeId;
    }

    QString StaticType::name() const
    {
        return QMetaType::typeName(_typeId);
    }

    IPropertyCSP StaticType::property(const QString& name) const
    {
        IPropertyWrapperCSP p = _properties.value(name);
        IPropertyCSP cp = std::static_pointer_cast<const IProperty>(p);
        return cp;
    }

    IPropertyWrapperCSP StaticType::propertyWrapper(const QString& name) const
    {
        return _properties.value(name);
    }

    QStringList StaticType::properties() const
    {
        return _properties.keys();
    }

    bool StaticType::isValid() const
    {
        return _typeId != QMetaType::UnknownType;
    }

    bool StaticType::operator == (const IType& other) const
    {
        return _typeId == other.typeId();
    }

    bool StaticType::fromJson(QVariant& value, const QJsonValue& json) const
    {
        QJsonObject object = json.toObject();
        for (auto propertyName : properties())
        {
            auto iter = object.find(propertyName);
            if (iter == object.end())
                continue;

            propertyWrapper(propertyName)->fromJsonValue(value.data(), iter.value());
        }
        return true;
    }

    QJsonValue StaticType::toJson(const QVariant& value) const
    {
        QJsonObject json;
        for (auto propertyName : properties())
        {
            json.insert(propertyName, propertyWrapper(propertyName)->toJsonValue(value.data()));
        }
        return json;
    }
}//namespace Qmt
