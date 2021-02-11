#include "qmt.h"

namespace Qmt
{
    ITypeSP MemEntity::type() const
    {
        return _type;
    }

    void MemEntity::setType(ITypeSP type)
    {
        _type = type;
    }

    QVariant MemEntity::property(const int index) const
    {
        return ;
    }

    void MemEntity::setProperty(const int index, const QVariant& value)
    {
    }



    int MemEntityCollection::entityCount() const
    {
        return _entities.size();
    }

    IEntitySP MemEntityCollection::entity(const int index) const
    {
        return _entities[index];
    }

    void MemEntityCollection::push_back(IEntitySP entity)
    {
        _entities.push_back(entity);
    }

    void MemEntityCollection::insert(const int index, IEntitySP entity)
    {
        _entities.insert(_entities.begin() + index, entity);
    }

    void MemEntityCollection::remove(const int index)
    {
        _entities.erase(_entities.begin() + index);
    }
}
// namespace Qmt
