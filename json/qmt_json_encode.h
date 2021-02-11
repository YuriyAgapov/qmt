#pragma once

#include "qmt_json_codec_type.h"
#include "qmt_string_tools.h"

#include <core/qmt_type_storage.h>

#include <QJsonArray>
#include <QJsonObject>

#include <list>
#include <vector>
#include <map>
#include <unordered_map>

namespace Qmt
{
    /// Прототип функции преобразования значения произвольного типа с метаинформацией в QJsonValue
    template<typename ValueType>
    inline QJsonValue toJsonValue(const ValueType& value);

    /// Базовая функция преобразования данных в общем виде к json
    inline QJsonValue variantValueToJson(const QVariant& value)
    {
        const int typeId = value.userType();
        if (typeId >= QMetaType::User)
        {
            auto type = getType(typeId);
            if (type && type->isValid())
                return type->toJson(value);
        }
        return QJsonValue::fromVariant(value);
    }

    /// Прототип функционального объекта для упаковки значения
    template<ValueCodecType, typename ValueType>
    struct EncodeJsonValue;

    /// Реализация упаковки "плоских" контейнеров
    template<typename ValueType>
    struct EncodeJsonValue<ArrayCodec, ValueType>
    {
        inline QJsonValue operator () (const ValueType& container)
        {
            QJsonArray array;
            for (const typename ValueType::value_type& value : container)
                array.append(toJsonValue(value));
            return array;
        }
    };

    /// Реализация упаковки ассоциативных контейнеров stl
    template<typename ValueType>
    struct EncodeJsonValue<StlAssociativeCodec, ValueType>
    {
        inline QJsonValue operator () (const ValueType& container)
        {
            QJsonObject object;
            for (auto iter = std::cbegin(container), end = std::cend(container); iter != end; ++iter)
                object[toString(iter->first)] = toJsonValue(iter->second);
            return object;
        }
    };

    /// Реализация упаковки ассоциативных контейнеров qtl
    template<typename ValueType>
    struct EncodeJsonValue<QtAssociativeCodec, ValueType>
    {
        inline QJsonValue operator () (const ValueType& container)
        {
            QJsonObject object;
            for (auto iter = std::cbegin(container), end = std::cend(container); iter != end; ++iter)
                object[toString(iter.key())] = toJsonValue(*iter);
            return object;
        }
    };

    /// Реализация упаковки в QJsonValue с использованием неявного преобразования
    template<typename ValueType>
    struct EncodeJsonValue<JsonCodec, ValueType>
    {
        inline QJsonValue operator () (const ValueType& value)
        {
            return value;
        }
    };

    /// Реализация упаковки значения через QVariant
    template<typename ValueType>
    struct EncodeJsonValue<ValueCodec, ValueType>
    {
        inline QJsonValue operator () (const ValueType& value)
        {
            return variantValueToJson(QVariant::fromValue(value));
        }
    };

    /// Реализация упаковки значения через QVariant
    template<typename ValueType>
    struct EncodeJsonValue<VariantCodec, ValueType>
    {
        inline QJsonValue operator () (const QVariant& value)
        {
            QJsonArray array;
            array.push_back(value.typeName());
            array.push_back(variantValueToJson(QVariant(value.userType(), value.data())));
            return array;
        }
    };

    /// Реализация упаковки значения через кодек
    template<typename ValueType>
    struct EncodeJsonValue<EntitySPCodec, ValueType>
    {
        inline QJsonValue operator () (const IEntitySP& value)
        {
            return value->toJson();
        }
    };

    /// Функция позволяет упаковать объект в QJsonValue, если это возможно, упаковка производиться
    /// через конструктор QJsonValue иначе через QVariant.
    /// В качестве описания для пользовательских типов используетя Qmt::Type.
    template<typename ValueType>
    inline QJsonValue toJsonValue(const ValueType& value)
    {
        return EncodeJsonValue<GetCodecType<ValueType>::type, ValueType>()(value);
    }
}
