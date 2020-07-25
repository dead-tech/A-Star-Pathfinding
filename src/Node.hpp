#ifndef NODE_H
#define NODE_H
#define __NODISCARD [[nodiscard]]

#include <stdint.h>
#include <memory>

#include "Vectors.hpp"

/**
 * @brief Node Class which is fundamental for the purpose of this project.
 * 
 */
class Node {
public:
    /**
     * @brief Used to check if the node is an endNode.
     * 
     */
    bool m_isEndNode = false;
    /**
     * @brief Used to check if the node is a startNode.
     * 
     */
    bool m_isStartNode = false;

    /**
     * @brief Construct a new Node object with default values, 0.0, 0.0 for y m_nodeCoordinates and true for m_walkable. 
     * @see m_nodeCoordinates
     * @see m_walkable
     */
    Node()
        : m_nodeCoordinates({ 0.0, 0.0 })
        , m_walkable { true }
    {
    }

    /**
     * @brief Construct a new Node object from a coordinates param and a walkable param
     * 
     * @param coordinates This is were the node is located in space, represented with a Vector<double, 2> (m_nodeCoordinates).
     * @param walkable This represent whether the node is an obstacle or can be walked on (m_walkable).
     */
    Node(const Vector<double, 2>& coordinates, const bool& walkable)
        : m_nodeCoordinates { coordinates }
        , m_walkable { walkable }
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
        return (this->X() == rhs.X() && this->Y() == rhs.Y());
    }

    /**
     * @see m_nodeCoordinates
     * @return Node's x position in space.
     */
    __NODISCARD const double X() const { return m_nodeCoordinates.X(); }
    /**
     * 
     * @see m_nodeCoordinates
     * @return Node's y position in space.
     */
    __NODISCARD const double Y() const { return m_nodeCoordinates.Y(); }

    /**
     * @see m_walkable
     * @return m_walkable
     */
    __NODISCARD const bool isWalkable() const { return m_walkable; }


private:
    /**
     * @brief Represents where the node is located in space with x, y coordinates.
     * @see Vector
     */
    const Vector<double, 2> m_nodeCoordinates;
    /**
     * @brief Defines whether a node is an obstacle or not.
     * 
     */
    const bool m_walkable;
};
#endif
