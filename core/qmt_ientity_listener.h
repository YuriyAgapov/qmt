#pragma once

namespace Qmt
{
    class IEntity;

    class IEntityListener
    {
    public:
        virtual ~IEntityListener() = default;

        virtual void aboutChanging(IEntity& entity) = 0;
        virtual void changed(IEntity& entity) = 0;
    };
}//namespace Qmt
