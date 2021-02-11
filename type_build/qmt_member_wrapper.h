#pragma once

#include "qmt_iproperty_wrapper.h"

#include <core/qmt_type_storage.h>
#include <json/qmt_json.h>

namespace Qmt
{
    template <typename ClassType, typename MemberType>
    class MemberWrapper : public IPropertyWrapper
    {
    public:
        MemberWrapper(const QString& name, MemberType ClassType::*member)
            : _name(name)
            , _typeId(qMetaTypeId<MemberType>())
            , _member(member)
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
            return QVariant::fromValue(reinterpret_cast<const ClassType*>(object)->*_member);
        }
        void fromVariant(void* object, const QVariant& value) const override
        {
            reinterpret_cast<ClassType*>(object)->*_member = value.value<MemberType>();
        }
        bool debug(QDebug& debug, const void* object) const override
        {
            debug << reinterpret_cast<const ClassType*>(object)->*_member;
            return true;
        }
        QJsonValue toJsonValue(const void* object) const override
        {
            return Qmt::toJsonValue(reinterpret_cast<const ClassType*>(object)->*_member);
        }
        void fromJsonValue(void* object, const QJsonValue& json) const override
        {
            Qmt::fromJsonValue(reinterpret_cast<ClassType*>(object)->*_member, json);
        }
    private:
        const QString _name;
        const int _typeId;
        MemberType ClassType::*_member;
    };
}
