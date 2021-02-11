#pragma once

#include <core/qmt_ptr.h>
#include <core/qmt_ientity.h>
#include <core/qmt_iobject.h>
#include <core/qmt_notifier.h>

namespace Qmt
{
    class MemEntity : public IEntity
    {
    public:
        explicit MemEntity(ITypeSP type = nullptr);

        ITypeSP type() const override;
        void setType(ITypeSP type);

        void addListener(IEntityListenerWP listener);
        void removeListener(IEntityListenerWP listener);

    protected:
        void beginChanging();
        void endChanging();

        ITypeSP _type;
        Notifier<IEntityListener> _notifier;
    };

    class MemObject :
            public IObject,
            public MemEntity,
            public std::enable_shared_from_this<MemObject>
    {
    public:
        MemObject(ITypeSP type = nullptr);

        IEntitySP entity();
        QVariant property(const QString& name) const;
        bool setProperty(const QString& name, const QVariant& value);

        QVariant toVariant() const override;
        void fromVariant(const QVariant& value) override;
        QJsonValue toJson() const override;
        void fromJson(const QJsonValue& value) override;
        void debug(QDebug& debug) const override;

    private:
        QVariantHash _properties;
    };
}// namespace Qmt

