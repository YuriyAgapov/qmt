#pragma once

#include "qmt_iproperty_wrapper.h"

#include <core/qmt_type_storage.h>
#include <json/qmt_json.h>

namespace Qmt
{
    template <typename ClassType, typename ValueType>
    class MethodWrapper : public IPropertyWrapper
    {
    public:
        using Getter = ValueType (ClassType::*)() const;
        using Setter = void (ClassType::*)(const ValueType&);

        MethodWrapper(const QString& name, Getter getter, Setter setter = nullptr)
            : _name(name)
            , _typeId(qMetaTypeId<ValueType>())
            , _getter(getter)
            , _setter(setter)
        {
        }

        QString name() const override
        {
            return _name;
        }
        int typeId() const override
        {
            return _typeId;
        }
        QString typeName() const
        {
            return QMetaType::typeName(_typeId);
        }
        QVariant defaultValue() const override
        {
            return QVariant(_typeId);
        }
        QVariant toVariant(const void* object) const override
        {
            return QVariant::fromValue(getValue(object));
        }
        void fromVariant(void* object, const QVariant& value) const override
        {
            setValue(object, value.value<ValueType>());
        }
        bool debug(QDebug& debug, const void* object) const override
        {
            debug << getValue(object);
            return true;
        }
        QJsonValue toJsonValue(const void* object) const override
        {
            return Qmt::toJsonValue(getValue(object));
        }
        void fromJsonValue(void* object, const QJsonValue& json) const override
        {
            setValue(object, Qmt::fromJsonValue<ValueType>(json));
        }
    private:
        ValueType getValue(const void* object) const
        {
            return (reinterpret_cast<const ClassType*>(object)->*_getter)();
        }
        void setValue(void* object, const ValueType& value) const
        {
            if (!_setter)
                return;
            (reinterpret_cast<ClassType*>(object)->*_setter)(value);
        }

        const QString _name;
        const int _typeId;
        Getter _getter;
        Setter _setter;
    };
}


