#ifndef VECTORS_H
#define VECTORS_H

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

    [[nodiscard]] const Type& X() const
    {
        return data[0];
    }

    [[nodiscard]] const Type& Y() const
    {
        return data[1];
    }

    [[nodiscard]] const Type& Z() const
    {
        return data[2];
    }

private:
    std::array<Type, size> data;
};

#endif
