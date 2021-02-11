#pragma once

#include "qmt_idecoration_model.h"

namespace Qmt
{
    class SimpleDecorationModel : public IDecorationModel
    {
    public:
        SimpleDecorationModel();

        bool toView(QVariant& value, const int role,
                    const QModelIndex& index, const EntityModel& model) const override;

    private:
        int _iconIndex = 0;
    };
}//namespace Qmt
