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
    }

    /**
     * @brief Copy constructor is defaulted.
     * 
     * @param vector Vector to be copied.
     */
    Vector(const Vector& vector) = default;
    /**
     * @brief Move constructor is defaulted.
     * 
     * @param vector Vector to be moved.
     */
    Vector(Vector vector&&) = default;
    /**
     * @brief Assignment operator is defaulted.
     * 
     * @return Vector& a reference to the assigned Vector. 
     */
    Vector& operator=(const Vector&) = default;

    /**
     * @brief Returns the X coordinate of the Vector which is esentially the fist element in the Vector.
     * 
     * @see data
     * @return First element of the Vector. 
     */
    __NODISCARD const Type& X() const
    {
        return data[0];
    }

    /**
     * @brief Returns the Y coordinate of the Vector which is esentially the secondo element in the Vector.
     *
     * @see data 
     * @return Second element of the Vector.
     */
    __NODISCARD const Type& Y() const
    {
        return data[1];
    }

    /**
     * @brief Returns the Z coordinate of the Vector which is esentially the third element in the Vector.
     * 
     * @see data
     * @return Third element of the Vector.
     */
    __NODISCARD const Type& Z() const
    {
        return data[2];
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

#endif
