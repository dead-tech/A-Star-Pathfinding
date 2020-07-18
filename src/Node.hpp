#ifndef NODE_H
#define NODE_H


#include <stdint.h>
#include <memory>

#include "Vectors.hpp"


/*
    The idea here is that for each node we need to calculate
    m_gScore, m_heuristic and m_fScore starting from the starting
    node and continuing following the path of the nodes with
    the lowest m_fScore.

    Tip: Usually as regards the cost of the movements is used 14
    for diagonal movements and 10 for up, down, left and right ones.
*/

class Node {
public:
    //int32_t m_gScore    = 0; // The distance from the starting node.
    //int32_t m_heuristic = 0; // The distance from the end node.
    //int32_t m_fScore    = 0; // The sum of m_gScore and m_heuristic.
    bool m_isEndNode   = false;
    bool m_isStartNode = false;

    Node()
        : m_nodeCoordinates({ 0.0, 0.0 })
        , m_walkable(true)
    {
    }

    Node(const Vector<double, 2>& coordinates, const bool& walkable)
        : m_nodeCoordinates(coordinates)
        , m_walkable(walkable)
    {
    }

    [[nodiscard]] constexpr bool operator==(const Node& rhs) const
    {
        return (this->X() == rhs.X() && this->Y() == rhs.Y());
    }

    [[nodiscard]] constexpr double X() const { return m_nodeCoordinates.X(); }
    [[nodiscard]] constexpr double Y() const { return m_nodeCoordinates.Y(); }

    [[nodiscard]] constexpr bool isWalkable() const { return m_walkable; }


private:
    const Vector<double, 2> m_nodeCoordinates; // This represents where the node is located in the grid.
    const bool              m_walkable;
};
#endif
