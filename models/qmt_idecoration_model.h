#pragma once

#include <Qt>

#include <memory>

class QVariant;
class QModelIndex;

namespace Qmt
{
    class EntityModel;

    class IDecorationModel
    {
    public:
        virtual ~IDecorationModel() = default;

        /// return true if write to \a value decorative data for \a role
        virtual bool toView(QVariant& value, const int role,
                            const QModelIndex& index, const EntityModel& model) const = 0;
    };
}//namespace Qmt

