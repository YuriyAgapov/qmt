#pragma once

#include <QString>
#include <QVariant>
#include <QVariantHash>

#include <memory>

namespace Qmt
{
    class IEntity;

    class ITable
    {
    public:
        using IEntitySP = std::shared_ptr<IEntity>;

        virtual ~ITable() = default;

        virtual IEntitySP entity() = 0;
        virtual QVariant property(const QString& name, const size_t row) const = 0;
        virtual bool setProperty(const QString& name, const size_t row, const QVariant& value) = 0;
        virtual size_t size() const = 0;
        virtual void insert(const size_t row, const QVariantHash& values = QVariantHash()) = 0;
        virtual void remove(const size_t row, const size_t count = 1) = 0;
        virtual QVariantHash rowProperties(const size_t row) const = 0;
    };
}
