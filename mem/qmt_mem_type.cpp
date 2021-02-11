#include "qmt_mem_type.h"

#include <core/qmt_iproperty.h>

namespace Qmt
{

    MemType::MemType()
        : _typeId(QMetaType::UnknownType)
        , _name(QMetaType::typeName(_typeId))
    {
    }

    int MemType::typeId() const
    {
        return _typeId;
    }

    void MemType::setTypeId(const int typeId)
    {
        _typeId = typeId;
        _name = QMetaType::typeName(_typeId);
    }

    QString MemType::name() const
    {
        return _name;
    }

    void MemType::setName(const QString& name)
    {
        _name = name;
        _typeId = QMetaType::type(qPrintable(name));
    }

    IPropertyCSP MemType::property(const QString& name) const
    {
        return _properties[name];
    }

    void MemType::addProperty(IPropertySP property)
    {
        Q_ASSERT(property);

        _properties[property->name()] = property;
    }

    QStringList MemType::properties() const
    {
        return _properties.keys();
    }

    bool MemType::isValid() const
    {
        return _typeId != QMetaType::UnknownType;
    }

    bool MemType::operator ==(const IType& other) const
    {
        return _typeId == other.typeId();
    }
}
