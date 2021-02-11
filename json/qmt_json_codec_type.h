#pragma once

#include <core/qmt_ientity.h>

#include <QVector>
#include <QList>
#include <QMap>
#include <QHash>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>

namespace Qmt
{
    /// Варианты упаковки значений
    enum ValueCodecType
    {
        /// Сериализация и десериализация значения происходит напрямую
        /// тип должен поддерживать преобразование в строку (см QMetaType::registerConverter)
        ValueCodec = sizeof(uint8_t),

        /// Сериализация и десериализация значения с сохранением типа
        /// тип должен поддерживать преобразование в строку (см QMetaType::registerConverter)
        VariantCodec = sizeof(uint8_t[2]),

        /// Сериализация и десериализация значения происходит с использованием неявного преобразования
        JsonCodec = sizeof(uint8_t[3]),

        /// Сериализация и десериализация контейнера (QJsonArray)
        ArrayCodec = sizeof(uint8_t[4]),

        /// Сериализация и десериализация ассоциативного контейнера (в QJsonObject)
        StlAssociativeCodec = sizeof(uint8_t[5]),

        /// Сериализация и десериализация ассоциативного контейнера (в QJsonObject)
        QtAssociativeCodec = sizeof(uint8_t[6]),

        EntitySPCodec = sizeof(uint8_t[7])
    };

    template <typename Type>
    struct GetCodecType
    {
    private:
        template <typename X>
        struct test { static const uint8_t value; };
        template <typename X>
        struct test<QVector<X>> { static const uint8_t value[ArrayCodec]; };
        template <typename X>
        struct test<QList<X>> { static const uint8_t value[ArrayCodec]; };
        template <typename X>
        struct test<std::list<X>> { static const uint8_t value[ArrayCodec]; };
        template <typename X>
        struct test<std::vector<X>> { static const uint8_t value[ArrayCodec]; };
        template <typename X, typename Y>
        struct test<std::map<X, Y>> { static const uint8_t value[StlAssociativeCodec]; };
        template <typename X, typename Y>
        struct test<std::unordered_map<X, Y>> { static const uint8_t value[StlAssociativeCodec]; };
        template <typename X, typename Y>
        struct test<QMap<X, Y>> { static const uint8_t value[QtAssociativeCodec]; };
        template <typename X, typename Y>
        struct test<QHash<X, Y>> { static const uint8_t value[QtAssociativeCodec]; };
    public:
        static const ValueCodecType type = std::is_same<Type, QVariant>::value
        ? VariantCodec
        : std::is_same<Type, IEntitySP>::value
            ? EntitySPCodec
            : (std::is_constructible<QJsonValue, Type>::value
               || std::is_assignable<QJsonValue, Type>::value
               || std::is_assignable<QJsonObject, Type>::value)
              ? JsonCodec
              : static_cast<ValueCodecType>(sizeof(test<Type>::value));
    };


}
