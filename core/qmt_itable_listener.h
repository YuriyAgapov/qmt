#pragma once

#include "qmt_ientity_listener.h"

namespace Qmt
{
    class ITable;

    class ITableListener : public IEntityListener
    {
    public:
        virtual void rowsAboutToBeInserted(ITable& table, const size_t start, const size_t last) = 0;
        virtual void rowsInserted(ITable& table, const size_t start, const size_t last) = 0;
        virtual void rowsAboutToBeRemoved(ITable& table, const size_t start, const size_t last) = 0;
        virtual void rowsRemoved(ITable& table, const size_t start, const size_t last) = 0;
    };
}//namespace Qmt
