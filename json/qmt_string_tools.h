#pragma once

#include <QString>

namespace Qmt
{
    template <typename T>
    QString toString(const T& value);

    template <>
    inline QString toString<QString>(const QString& value)
        { return value; }

    template <>
    inline QString toString<std::string>(const std::string& value)
        { return QString::fromStdString(value); }
}
