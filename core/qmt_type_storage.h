#pragma once

#include "qmt_itype.h"

#include <memory>

#include "qmt_ptr.h"

#include <QDebug>
#include <QHash>
#include <QVariant>

#include <functional>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>

namespace Qmt
{
    class TypeStorage
    {
    public:
        TypeStorage();

        void add(ITypeSP type);
        ITypeSP get(const int id) const;

        static TypeStorageSP instance();

    private:
        std::unordered_map<int, ITypeSP> types;
    };

    inline void registerType(ITypeSP type)
    {
        TypeStorage::instance()->add(type);
    }
    inline ITypeSP getType(const int typeId)
    {
        return TypeStorage::instance()->get(typeId);
    }

    template <typename TypeType>
    inline std::shared_ptr<TypeType> getTypeOf(const int typeId)
    {
        return std::dynamic_pointer_cast<TypeType>(TypeStorage::instance()->get(typeId));
    }

    template <typename ClassType>
    inline ITypeSP getType()
    {
        return getType(qMetaTypeId<ClassType>());
    }

    template <typename TypeType, typename ClassType>
    inline std::shared_ptr<TypeType> getTypeOf()
    {
        return getTypeOf<TypeType>(qMetaTypeId<ClassType>());
    }
}
