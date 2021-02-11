#pragma once

#include <core/qmt_itype.h>
#include <core/qmt_ptr.h>

#include <QHash>

namespace Qmt
{
    class MemType : public IType
    {
    public:
        MemType();

        int typeId() const override;
        void setTypeId(const int typeId);

        QString name() const override;
        void setName(const QString& name);

        IPropertyCSP property(const QString& name) const override;
        void addProperty(IPropertySP property);

        QStringList properties() const override;
        bool isValid() const override;
        bool operator == (const IType& other) const override;

    private:
        int _typeId;
        QString _name;
        QHash<QString, IPropertySP> _properties;
    };
}// namespace Qmt

