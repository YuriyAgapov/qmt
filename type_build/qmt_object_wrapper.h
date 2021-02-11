#pragma once

#include "qmt_value_wrapper.h"

#include <core/qmt_iobject.h>
#include <core/qmt_iobject_listener.h>

#include <QString>
#include <QVariant>
#include <QVariantHash>

namespace Qmt
{
    class IEntity;

    template <typename ClassType>
    class ObjectWrapper :
            public ValueWrapper<ClassType>,
            public IObject,
            public std::enable_shared_from_this<ObjectWrapper<ClassType> >
    {
    public:
        ObjectWrapper(ClassType& value);

        IEntitySP entity() override;
        QVariant property(const QString& name) const override;
        bool setProperty(const QString& name, const QVariant& value) override;

    private:
        using BaseEntity = ValueWrapper<ClassType>;

        void aboutPropertyChanging(const QString& name, const QVariant& value);
        void propertyChanged(const QString& name, const QVariant& value);
    };

    template<typename ClassType>
    ObjectWrapper<ClassType>::ObjectWrapper(ClassType& value) : ValueWrapper<ClassType>(value)
    {
    }

    template<typename ClassType>
    IObject::IEntitySP ObjectWrapper<ClassType>::entity()
    {
        return std::enable_shared_from_this<ObjectWrapper<ClassType> >::shared_from_this();
    }

    template<typename ClassType>
    inline QVariant ObjectWrapper<ClassType>::property(const QString& name) const
    {
        auto property = ValueWrapper<ClassType>::_type->propertyWrapper(name);
        return property ? property->toVariant(&(BaseEntity::_value))
                        : QVariant();
    }

    template<typename ClassType>
    inline bool ObjectWrapper<ClassType>::setProperty(const QString& name, const QVariant& value)
    {
        auto property = BaseEntity::_type->propertyWrapper(name);
        if (!property)
            return false;

        aboutPropertyChanging(name, value);
        property->fromVariant(&(BaseEntity::_value), value);
        propertyChanged(name, value);
        return true;
    }

    template<typename ClassType>
    void ObjectWrapper<ClassType>::aboutPropertyChanging(const QString& name, const QVariant& value)
    {
        Notifier<IEntityListener>& notifier = BaseEntity::_notifier;
        notifier.for_each<IObjectListener>([this, name, value](auto& listener)
        {
            listener.aboutPropertyChanging(*this, name, value);
        });
    }

    template<typename ClassType>
    void ObjectWrapper<ClassType>::propertyChanged(const QString& name, const QVariant& value)
    {
        Notifier<IEntityListener>& notifier = BaseEntity::_notifier;
        notifier.for_each<IObjectListener>([this, name, value](IObjectListener& listener)
        {
            listener.propertyChanged(*this, name, value);
        });
    }
}
