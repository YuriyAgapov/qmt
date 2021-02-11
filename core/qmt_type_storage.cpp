#include "qmt_type_storage.h"

#include "qmt_iproperty.h"

namespace Qmt
{
    TypeStorage::TypeStorage()
    {
    }

    void TypeStorage::add(ITypeSP type)
    {
        types[type->typeId()] = type;
    }

    ITypeSP TypeStorage::get(const int id) const
    {
        auto iter = types.find(id);
        return iter == types.end() ? nullptr
                                   : iter->second;
    }

    TypeStorageSP TypeStorage::instance()
    {
        static TypeStorageSP storage = std::make_shared<TypeStorage>();
        return storage;
    }
}
