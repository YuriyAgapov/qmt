#include "qmt_mem_entity.h"

#include <core/qmt_ientity_listener.h>
#include <core/qmt_itype.h>
#include <json/qmt_json.h>

namespace Qmt
{
    MemEntity::MemEntity(ITypeSP type)
        : _type(type)
    {
    }

    ITypeSP MemEntity::type() const
    {
        return _type;
    }

    void MemEntity::setType(ITypeSP type)
    {
        _type = type;
    }

    void MemEntity::addListener(IEntityListenerWP listener)
    {
        _notifier.addListener(listener);
    }

    void MemEntity::removeListener(IEntityListenerWP listener)
    {
        _notifier.removeListener(listener);
    }

    void MemEntity::beginChanging()
    {
        _notifier.for_each<IEntityListener>([this](auto& listener)
        {
            listener.aboutChanging(*this);
        });
    }

    void MemEntity::endChanging()
    {
        _notifier.for_each<IEntityListener>([this](auto& listener)
        {
            listener.changed(*this);
        });
    }

    //----------

    IObject::IEntitySP MemObject::entity()
    {
        return shared_from_this();
    }

    QVariant MemObject::property(const QString& name) const
    {
        return _properties[name];
    }

    bool MemObject::setProperty(const QString& name, const QVariant& value)
    {
        _properties[name] = value;
        return true;
    }

    QVariant MemObject::toVariant() const
    {
        return _properties;
    }

    void MemObject::fromVariant(const QVariant& value)
    {
        _properties = value.toHash();
    }

    QJsonValue MemObject::toJson() const
    {
        return toJsonValue(_properties);
    }

    void MemObject::fromJson(const QJsonValue& value)
    {
        _properties = fromJsonValue<QVariantHash>(value);
    }

    void MemObject::debug(QDebug& debug) const
    {
        debug << _properties;
    }
} // namespace Qmt
