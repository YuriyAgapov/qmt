#pragma once

#include "qmt_ientity_listener.h"
#include <QVariant>

class QString;

namespace Qmt
{
    class IObject;

    class IObjectListener : public IEntityListener
    {
    public:
        virtual QVariant aboutPropertyChanging(IObject& /*object*/,
                                               const QString& /*name*/,
                                               const QVariant& value) { return value; }
        virtual void propertyChanged(IObject& /*object*/,
                                     const QString& /*name*/,
                                     const QVariant& /*value*/) {}
    };
}//namespace Qmt
