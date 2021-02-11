#pragma once

#include <memory>

#define QMT_DECLARE_PTR(Type) \
    using Type##SP = std::shared_ptr<Type>; \
    using Type##WP = std::weak_ptr<Type>; \
    using Type##CSP = std::shared_ptr<const Type>;

#define QMT_PREDECLARE_PTR(Type) \
    class Type; \
    QMT_DECLARE_PTR(Type)
