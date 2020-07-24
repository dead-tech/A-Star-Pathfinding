#ifndef VECTORS_H
#define VECTORS_H
#define __NODISCARD [[nodiscard]]

#include <array>

template <typename Type, std::size_t size>
struct Vector {

    template <typename... Args>
    Vector(Args... args)
        : data { args... }
    {
    }

    Vector(const Vector& vector) = default;
    Vector(Vector&&)             = default;
    Vector& operator=(const Vector&) = default;

    __NODISCARD const Type& X() const
    {
        return data[0];
    }

    __NODISCARD const Type& Y() const
    {
        return data[1];
    }

    __NODISCARD const Type& Z() const
    {
        return data[2];
    }

private:
    std::array<Type, size> data;
};

#endif
