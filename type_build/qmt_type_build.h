#pragma once

#include "qmt_member_wrapper.h"
#include "qmt_method_wrapper.h"
#include "qmt_static_type.h"

namespace Qmt
{
    template <typename ClassType, typename MemberType>
    IPropertyWrapperSP makeProperty(const QString& name, MemberType ClassType::*member)
    {
        return std::make_shared<MemberWrapper<ClassType, MemberType> >(name, member);
    }
    template <typename ClassType, typename ValueType, typename Getter, typename Setter>
    IPropertyWrapperSP makeProperty(const QString& name, Getter getter, Setter setter = nullptr)
    {
        return std::make_shared<MethodWrapper<ClassType, ValueType> >(name, getter, setter);
    }

    /// \name Сравнивает значения по декларированным параметрам
    inline bool compare(const QVariant& l, const QVariant& r, const int typeId);
    inline bool compare(const void* l, const void* r, StaticTypeSP type)
    {
        Q_ASSERT(type && type->isValid());

        if (!type || !type->isValid())
            return false;

        for (auto propertyName : type->properties())
        {
            auto property = type->propertyWrapper(propertyName);
            if (!compare(property->toVariant(l), property->toVariant(r), property->typeId()))
                return false;
        }
        return true;
    }

    inline bool compare(const QVariant& l, const QVariant& r, const int typeId)
    {
        if (typeId != qMetaTypeId<QVariant>())
        {
            Q_ASSERT(typeId == l.userType());
            Q_ASSERT(typeId == r.userType());
        }

        if (l.userType() != r.userType())
        {
            return false;
        }

        if (typeId < static_cast<int>(QVariant::UserType))
        {
            return l == r;
        }

        return compare(l.data(), r.data(), getTypeOf<StaticType>(typeId));
    }

    template <typename ClassType>
    bool compare(ClassType l, ClassType r)
    {
        const int typeId = qMetaTypeId<ClassType>();
        ITypeSP type = getType(typeId);
        return type && type->isValid() ? compare(&l, &r, type)
                                       : compare(QVariant::fromValue(l), QVariant::fromValue(r), typeId);
    }
    ///@}

    /// Декларировать оператор сравнения для заданного типа
    #define QMT_DECLARE_EQUALY_OPERATOR(ClassType) \
        inline bool operator == (const ClassType& l, const ClassType& r) \
            { return Qmt::compare(&l, &r, Qmt::getTypeOf<Qmt::StaticType, ClassType>()); }
}
