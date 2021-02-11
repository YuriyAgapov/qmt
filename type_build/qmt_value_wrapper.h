#pragma once

#include "qmt_static_type.h"

#include <core/qmt_ientity.h>
#include <core/qmt_ientity_listener.h>
#include <core/qmt_iproperty.h>
#include <core/qmt_notifier.h>
#include <core/qmt_type_storage.h>
#include <json/qmt_json.h>

#include <QString>
#include <QVariant>

#include <functional>
#include <memory>

namespace Qmt
{
    template <typename ClassType>
    class ValueWrapper : public IEntity
    {
    public:
        ValueWrapper(ClassType& value);

        ITypeSP type() const override;
        QVariant toVariant() const override;
        void fromVariant(const QVariant& value) override;
        QJsonValue toJson() const override;
        void fromJson(const QJsonValue& value) override;
        void debug(QDebug& debug) const override;

        ClassType& value();
        const ClassType& value() const;
        void setValue(const ClassType& value);

        void addListener(IEntityListenerWP listener);
        void removeListener(IEntityListenerWP listener);

    protected:
        ValueWrapper(StaticTypeSP type, ClassType& value);

        void beginChanging();
        void endChanging();

        StaticTypeSP _type;
        ClassType& _value;
        Notifier<IEntityListener> _notifier;
    };

    template<typename ClassType>
    inline ValueWrapper<ClassType>::ValueWrapper(ClassType& value)
        : ValueWrapper(getTypeOf<StaticType, ClassType>(), value)
    {
    }

    template<typename ClassType>
    ITypeSP ValueWrapper<ClassType>::type() const
    {
        return _type;
    }

    template<typename ClassType>
    inline QVariant ValueWrapper<ClassType>::toVariant() const
    {
        return QVariant::fromValue(_value);
    }

    template<typename ClassType>
    inline void ValueWrapper<ClassType>::fromVariant(const QVariant& value)
    {
        beginChanging();
        _value = value.value<ClassType>();
        endChanging();
    }

    template<typename ClassType>
    inline QJsonValue ValueWrapper<ClassType>::toJson() const
    {
        return toJsonValue(_value);
    }

    template<typename ClassType>
    inline void ValueWrapper<ClassType>::fromJson(const QJsonValue& value)
    {
        beginChanging();
        fromJsonValue(_value, value);
        endChanging();
    }

    template<typename ClassType>
    inline void ValueWrapper<ClassType>::debug(QDebug& debug) const
    {
        debug << _value;
    }

    template<typename ClassType>
    ClassType& ValueWrapper<ClassType>::value()
    {
        return _value;
    }

    template<typename ClassType>
    const ClassType& ValueWrapper<ClassType>::value() const
    {
        return _value;
    }

    template<typename ClassType>
    void ValueWrapper<ClassType>::setValue(const ClassType& value)
    {
        beginChanging();
        _value = value;
        endChanging();
    }

    template<typename ClassType>
    void ValueWrapper<ClassType>::addListener(IEntityListenerWP listener)
    {
        _notifier.addListener(listener);
    }

    template<typename ClassType>
    void ValueWrapper<ClassType>::removeListener(IEntityListenerWP listener)
    {
        _notifier.removeListener(listener);
    }

    template<typename ClassType>
    ValueWrapper<ClassType>::ValueWrapper(StaticTypeSP type, ClassType& value)
        : _type(type)
        , _value(value)
    {
    }

    template<typename ClassType>
    void ValueWrapper<ClassType>::beginChanging()
    {
        _notifier.for_each<IEntityListener>([this](auto& listener)
        {
            listener.aboutChanging(*this);
        });
    }

    template<typename ClassType>
    void ValueWrapper<ClassType>::endChanging()
    {
        _notifier.for_each<IEntityListener>([this](auto& listener)
        {
            listener.changed(*this);
        });
    }
}
