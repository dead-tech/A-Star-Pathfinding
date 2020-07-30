#ifndef VECTORS_H
#define VECTORS_H
#define __NODISCARD [[nodiscard]]

#include <array>
/**
 * @brief Vector struct that can hold size data of type Type  
 * 
 * @tparam Type The type of the elements in the Vector
 * @tparam size The maximum size of the Vector
 */

template <typename Type, std::size_t size>
struct Vector {

    Type x;
    Type y;
    Type z;

    /**
     * @brief Variadic templated constructor that accepts at least one parameter.
     * 
     * @tparam Args The type of the elements of the Vector.
     * @param args The effective elements to be stored in the Vector.
     */
    template <typename... Args>
    Vector(Args... args)
        : data { args... }
    {
        x = data[0];
        y = data[1];
        if (size > 2) {
            z = data[2];
        }
        z = 0;
    }

    __NODISCARD const bool operator==(const Vector<Type, 2> rhs) const
    {
        /* bool res = (x == rhs.x) && (y == rhs.y);
        return size == 3 ? res && (z = rhs.z) : res;*/
        return (x == rhs.x) && (y == rhs.y);
    }

    __NODISCARD const bool operator==(const Vector<Type, 3> rhs) const
    {
        return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
    }

private:
    /**
     * @brief Underlying data structure of the Vector.
     * 
     * @tparam Type std::arrays' elements type.
     * @tparam size std::arrays' size.
     */
    std::array<Type, size> data;
};

namespace std {

template <>
struct hash<Vector<double, 2>> {
    std::size_t operator()(const Vector<double, 2>& coords) const
    {
        return ((hash<double>()(coords.x)) ^ (hash<double>()(coords.y) << 1));
    }
};

}

#endif
