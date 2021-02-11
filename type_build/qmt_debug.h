#pragma once

#include "qmt_iproperty_wrapper.h"
#include "qmt_static_type.h"
#include "qmt_type_build_ptr.h"

#include <core/qmt_iproperty.h>
#include <core/qmt_itype.h>
#include <core/qmt_ptr.h>
#include <core/qmt_type_storage.h>

#include <QDebug>

#include <memory>

namespace Qmt
{
    template <typename ClassType>
    QDebug& printProperties(QDebug& debug,
                            const ClassType& object,
                            StaticTypeSP type)
    {
        for (const auto& propertyName : type->properties())
        {
            auto property = type->propertyWrapper(propertyName);
            debug << qPrintable(property->name()) << ":";
            property->debug(debug, &object);
            if (propertyName != type->properties().back())
                debug << ",";
        }

        return debug;
    }

    template <typename ClassType>
    inline QDebug debugOperator(QDebug& debug, const ClassType& value, StaticTypeSP type)
    {
        debug.nospace() << qPrintable(type->name()) << "(";
        printProperties(debug, value, type);
        debug << ")";
        return debug;
    }

    #define QMT_DECLARE_DEBUG_STREAM_OPERATOR(ClassType) \
    inline QDebug operator << (QDebug debug, const ClassType& value) \
    { return debugOperator(debug, value, Qmt::getTypeOf<Qmt::StaticType, ClassType>()); }
}
