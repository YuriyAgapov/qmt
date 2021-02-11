#pragma once

#include "qmt_json_codec_type.h"
#include "qmt_string_tools.h"

#include <core/qmt_ientity.h>
#include <core/qmt_type_storage.h>

#include <QJsonArray>
#include <QJsonObject>

#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Qmt
{
    /// Прочитать в \a value из \a json
    template <typename ValueType>
    void fromJsonValue(ValueType& value, const QJsonValue& json);

    template <typename ValueType>
    void variantValueFromJson(ValueType& value, const QJsonValue& json)
    {
        const int typeId = qMetaTypeId<ValueType>();
        if (typeId >= QMetaType::User)
        {
            auto type = getType(typeId);
            if (type && type->isValid())
            {
                QVariant tmp(typeId, nullptr);
                type->fromJson(tmp, json);
                value = tmp.value<ValueType>();
                return;
            }
        }
        value = json.toVariant().value<ValueType>();
    }

    /// Прототип функционального объекта для упаковки значения
    template<ValueCodecType, typename ValueType>
    struct DecodeJsonValue;

    /// Реализация упаковки "плоских" контейнеров
    template<typename ValueType>
    struct DecodeJsonValue<ArrayCodec, ValueType>
    {
        static void decode(ValueType& container, const QJsonValue& json)
        {
            for (const auto& jsonValue : json.toArray())
            {
                typename ValueType::value_type value;
                fromJsonValue(value, jsonValue);
                container.push_back(value);
            }
        }
    };

    /// Реализация упаковки ассоциативных контейнеров stl (unordered_map, map)
    template<typename ValueType>
    struct DecodeJsonValue<StlAssociativeCodec, ValueType>
    {
        static void decode(ValueType& container, const QJsonValue& json)
        {
            const QJsonObject jsonObject = json.toObject();
            for (auto iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)
            {
                typename ValueType::mapped_type value;
                fromJsonValue(value, iter.value());
                container[iter.key().toStdString()] = value;
            }
        }
    };

    /// Реализация упаковки ассоциативных контейнеров qtl (map, hash)
    template<typename ValueType>
    struct DecodeJsonValue<QtAssociativeCodec, ValueType>
    {
        static void decode(ValueType& container, const QJsonValue& json)
        {
            const QJsonObject jsonObject = json.toObject();
            for (auto iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)
            {
                typename ValueType::mapped_type value;
                fromJsonValue(value, iter.value());
                container[iter.key()] = value;
            }
        }
    };

    /// Прототип функционального объекта для упаковки значения
    template<typename ValueType>
    struct DecodeJsonValue<ValueCodec, ValueType>
    {
        static void decode(ValueType& value, const QJsonValue& json)
        {
            variantValueFromJson(value, json);
        }
    };

    /// Прототип функционального объекта для упаковки значения
    template<typename ValueType>
    struct DecodeJsonValue<VariantCodec, ValueType>
    {
        static void decode(ValueType& value, const QJsonValue& json)
        {
            const QJsonArray array = json.toArray();
            const int typeId = QMetaType::type(qPrintable(array[0].toString()));
            if (typeId >= QMetaType::User)
            {
                value = QVariant(typeId, nullptr);
                auto type = getType(typeId);
                type->fromJson(value, array[1]);
            }
            else
            {
                value = array[1].toVariant();
            }
        }
    };

    /// Прототип функционального объекта для упаковки значения
    template<typename ValueType>
    struct DecodeJsonValue<JsonCodec, ValueType>
    {
        static void decode(ValueType& value, const QJsonValue& json)
        {
            variantValueFromJson(value, json);
        }
    };

    /// Прототип функционального объекта для упаковки значения
    template<typename ValueType>
    struct DecodeJsonValue<EntitySPCodec, ValueType>
    {
        static void decode(IEntitySP& entity, const QJsonValue& json)
        {
            entity->fromJson(json);
        }
    };

    template <typename ValueType>
    inline void fromJsonValue(ValueType& value, const QJsonValue& json)
    {
        DecodeJsonValue<GetCodecType<ValueType>::type, ValueType>::decode(value, json);
    }

    template <typename ValueType>
    inline ValueType fromJsonValue(const QJsonValue& json)
    {
        ValueType value;
        DecodeJsonValue<GetCodecType<ValueType>::type, ValueType>::decode(value, json);
        return value;
    }
}
