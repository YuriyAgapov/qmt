#pragma once

#include "qmt_debug.h"
#include "qmt_static_type.h"
#include "qmt_value_wrapper.h"

#include <core/qmt_type_storage.h>
#include <type_build/qmt_type_build.h>

#include <QMetaType>

#include <memory>

#define QMT_DECLARE_TYPE_OPERATORS(T) \
    QMT_DECLARE_DEBUG_STREAM_OPERATOR(T) \
    QMT_DECLARE_EQUALY_OPERATOR(T)

#define QMT_DECLARE_METATYPE(T) \
    Q_DECLARE_METATYPE(T) \
    QMT_DECLARE_TYPE_OPERATORS(T) \

#define QMT_PROPERTY_MEMBER(T, m) {#m, Qmt::makeProperty<T>(#m, &T::m)}
#define QMT_PROPERTY_METHOD(T, m, Type, get, set) {#m, Qmt::makeProperty<T, Type>(#m, &T::get, &T::set)}

/// helpers
#define QMT_REGISTER_F(T, qmtType) \
        Qmt::registerType(qmtType); \
        QMetaType::registerEqualsComparator<T>(); \
        QMetaType::registerDebugStreamOperator<T>();

#define QMT_REGISTER(T) QMT_REGISTER_F(T, qmtMeta##T());

#define QMT_PROPERTIES2(T, m1, m2) \
        QMT_PROPERTY_MEMBER(T, m1), QMT_PROPERTY_MEMBER(T, m2)
#define QMT_PROPERTIES3(T, m1, m2, m3) \
        QMT_PROPERTIES2(T, m1, m2), QMT_PROPERTY_MEMBER(T, m3)
#define QMT_PROPERTIES4(T, m1, m2, m3, m4) \
        QMT_PROPERTIES3(T, m1, m2, m3), QMT_PROPERTY_MEMBER(T, m4)
#define QMT_PROPERTIES5(T, m1, m2, m3, m4, m5) \
        QMT_PROPERTIES4(T, m1, m2, m3, m4), QMT_PROPERTY_MEMBER(T, m5)
#define QMT_PROPERTIES6(T, m1, m2, m3, m4, m5, m6) \
        QMT_PROPERTIES5(T, m1, m2, m3, m4, m5), QMT_PROPERTY_MEMBER(T, m6)
#define QMT_PROPERTIES7(T, m1, m2, m3, m4, m5, m6, m7) \
        QMT_PROPERTIES6(T, m1, m2, m3, m4, m5, m6), QMT_PROPERTY_MEMBER(T, m7)
#define QMT_PROPERTIES8(T, m1, m2, m3, m4, m5, m6, m7, m8) \
        QMT_PROPERTIES7(T, m1, m2, m3, m4, m5, m6, m7), QMT_PROPERTY_MEMBER(T, m8)

#define QMT_DECLARE_TYPE_INFO(T, members) \
    Q_DECLARE_METATYPE(T) \
    inline std::shared_ptr<Qmt::IType> qmtMeta##T() \
    { \
        return std::make_shared<Qmt::StaticType>(qMetaTypeId<T>(), \
                                                 Qmt::StaticType::PropertyHash{members}); \
    } \
    QMT_DECLARE_TYPE_OPERATORS(T)

#define QMT_DECLARE_TYPE_INFO2(T, m1, m2) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES2(T, m1, m2))

#define QMT_DECLARE_TYPE_INFO3(T, m1, m2, m3) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES3(T, m1, m2, m3))

#define QMT_DECLARE_TYPE_INFO4(T, m1, m2, m3, m4) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES4(T, m1, m2, m3, m4))

#define QMT_DECLARE_TYPE_INFO5(T, m1, m2, m3, m4, m5) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES5(T, m1, m2, m3, m4, m5))

#define QMT_DECLARE_TYPE_INFO6(T, m1, m2, m3, m4, m5, m6) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES6(T, m1, m2, m3, m4, m5, m6))

#define QMT_DECLARE_TYPE_INFO7(T, m1, m2, m3, m4, m5, m6, m7) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES7(T, m1, m2, m3, m4, m5, m6, m7))

#define QMT_DECLARE_TYPE_INFO8(T, m1, m2, m3, m4, m5, m6, m7, m8) \
    QMT_DECLARE_TYPE_INFO(T, QMT_PROPERTIES8(T, m1, m2, m3, m4, m5, m6, m7, m8))
