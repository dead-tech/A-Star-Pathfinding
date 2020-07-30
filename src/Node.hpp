#ifndef NODE_H
#define NODE_H
#define __NODISCARD [[nodiscard]]


#include "Vectors.hpp"

/**
 * @brief Node Class which is fundamental for the purpose of this project.
 * 
 */
class Node {
public:
    bool walkable;

    /**
     * @brief Construct a new Node object with default values, 0.0, 0.0 for y m_nodeCoordinates and true for m_walkable.
     * @see m_nodeCoordinates
     * @see m_walkable
     */
    Node()
        : m_nodeCoordinates({ 0.0, 0.0 })
        , walkable { true }
    {
    }

    /**
     * @brief Construct a new Node object from a coordinates param and a walkable param
     * 
     * @param coordinates This is were the node is located in space, represented with a Vector<double, 2> (m_nodeCoordinates).
     * @param walkable This represent whether the node is an obstacle or can be walked on (m_walkable).
     */
    Node(const Vector<double, 2>& coordinates, const bool& walkable_)
        : m_nodeCoordinates { coordinates }
        , walkable { walkable_ }
    {
    }


    /**
     * @brief Compares two nodes based on m_nodeCoordinates x's and y's properties.
     * 
     * @param rhs The second node to make the comparison with.
     * @return Returns true if the nodes x and y coords are equal, false if they're not.
     */
    __NODISCARD const bool operator==(const Node& rhs) const
    {
        return m_nodeCoordinates == rhs.getCoords();
    }

    __NODISCARD const Vector<double, 2> getCoords() const { return m_nodeCoordinates; }


private:
    /**
     * @brief Represents where the node is located in space with x, y coordinates.
     * @see Vector
     */
    const Vector<double, 2> m_nodeCoordinates;
};

namespace std {

template <>
struct hash<Node> {
    std::size_t operator()(const Node& n) const
    {
        return ((hash<Vector<double, 2>>()(n.getCoords())) ^ (hash<bool>()(n.walkable) << 1));
    }
};

}

#endif
