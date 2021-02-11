#pragma once

#include "qmt_value_wrapper.h"
#include "qmt_iproperty_wrapper.h"

#include <core/qmt_itable.h>
#include <core/qmt_itable_listener.h>

#include <QString>
#include <QVariant>
#include <QVariantHash>

#include <memory>

namespace Qmt
{
    template <typename Collection>
    class TableWrapper :
            public ValueWrapper<Collection>,
            public ITable,
            public std::enable_shared_from_this<TableWrapper<Collection> >
    {
    public:
        TableWrapper(Collection& collection);

        IEntitySP entity() override;
        ITypeSP type() const override;
        QVariant property(const QString& name, const size_t index) const override;
        bool setProperty(const QString& name, const size_t index, const QVariant& value) override;
        size_t size() const override;
        void insert(const size_t row, const QVariantHash& values = QVariantHash()) override;
        void remove(const size_t row, const size_t count = 1) override;
        QVariantHash rowProperties(const size_t row) const override;

    private:
        using BaseEntity = ValueWrapper<Collection>;

        void rowsAboutToBeInserted(const size_t start, const size_t last);
        void rowsInserted(const size_t start, const size_t last);
        void rowsAboutToBeRemoved(const size_t start, const size_t last);
        void rowsRemoved(const size_t start, const size_t last);

        int _valueTypeId;
        StaticTypeSP _valueType;
    };

    class IEntity;
    template<typename Collection>
    TableWrapper<Collection>::TableWrapper(Collection& collection)
        : ValueWrapper<Collection>(collection)
        , _valueTypeId(qMetaTypeId<typename Collection::value_type>())
        , _valueType(getTypeOf<StaticType>(_valueTypeId))
    {
    }

    template<typename Collection>
    ITable::IEntitySP TableWrapper<Collection>::entity()
    {
        return std::enable_shared_from_this<TableWrapper<Collection> >::shared_from_this();
    }

    template<typename Collection>
    ITypeSP TableWrapper<Collection>::type() const
    {
        return _valueType;
    }

    template<typename Collection>
    QVariant TableWrapper<Collection>::property(const QString& name, const size_t index) const
    {
        Q_ASSERT(_valueType);
        auto property = _valueType->propertyWrapper(name);
        return property ? property->toVariant(&BaseEntity::_value.at(index))
                        : QVariant();
    }

    template<typename Collection>
    bool TableWrapper<Collection>::setProperty(const QString& name, const size_t index, const QVariant& value)
    {
        Q_ASSERT(_valueType);
        auto property = _valueType->propertyWrapper(name);
        if (!property)
            return false;

        property->fromVariant(&BaseEntity::_value[index], value);
        return true;
    }

    template<typename Collection>
    size_t TableWrapper<Collection>::size() const
    {
        return BaseEntity::_value.size();
    }

    template<typename Collection>
    void TableWrapper<Collection>::insert(const size_t row, const QVariantHash& values)
    {
        Q_ASSERT(_valueType);
        QVariant rowValue(_valueType->typeId(), nullptr);

        for (auto valueIter = values.begin(), valueEnd = values.end(); valueIter != valueEnd; ++valueIter)
        {
            auto property = _valueType->propertyWrapper(valueIter.key());
            if (!property)
                continue;

            property->fromVariant(rowValue.data(), valueIter.value());
        }
        rowsAboutToBeInserted(static_cast<int>(row), static_cast<int>(row));
        BaseEntity::_value.insert(BaseEntity::_value.begin() + row,
                                  rowValue.value<typename Collection::value_type>());
        rowsInserted(row, row);
    }

    template<typename Collection>
    void TableWrapper<Collection>::remove(const size_t row, const size_t count)
    {
        rowsAboutToBeRemoved(row, row);
        BaseEntity::_value.erase(BaseEntity::_value.begin() + row,
                                 BaseEntity::_value.begin() + row + count);
        rowsRemoved(row, row + count);
    }

    template<typename Collection>
    QVariantHash TableWrapper<Collection>::rowProperties(const size_t row) const
    {
        QVariantHash values;
        for (const auto& propertyName : _valueType->properties())
        {
            values[propertyName] = property(propertyName, row);
        }
        return values;
    }

    template<typename Collection>
    void TableWrapper<Collection>::rowsAboutToBeInserted(const size_t start, const size_t last)
    {
        Notifier<IEntityListener>& notifier = BaseEntity::_notifier;
        notifier.for_each<ITableListener>([this, start, last](ITableListener& listener)
        {
            listener.rowsAboutToBeInserted(*this, start, last);
        });
    }

    template<typename Collection>
    void TableWrapper<Collection>::rowsInserted(const size_t start, const size_t last)
    {
        Notifier<IEntityListener>& notifier = BaseEntity::_notifier;
        notifier.for_each<ITableListener>([this, start, last](ITableListener& listener)
        {
            listener.rowsInserted(*this, start, last);
        });
    }

    template<typename Collection>
    void TableWrapper<Collection>::rowsAboutToBeRemoved(const size_t start, const size_t last)
    {
        Notifier<IEntityListener>& notifier = BaseEntity::_notifier;
        notifier.for_each<ITableListener>([this, start, last](ITableListener& listener)
        {
            listener.rowsAboutToBeRemoved(*this, start, last);
        });
    }

    template<typename Collection>
    void TableWrapper<Collection>::rowsRemoved(const size_t start, const size_t last)
    {
        Notifier<IEntityListener>& notifier = BaseEntity::_notifier;
        notifier.for_each<ITableListener>([this, start, last](ITableListener& listener)
        {
            listener.rowsRemoved(*this, start, last);
        });
    }
}
