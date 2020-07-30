#ifndef EDGE_H
#define EDGE_H
#include "Node.hpp"

#include <memory>

#define __NODISCARD [[nodiscard]]

/**
 * @brief This Edge class helps with the implementation of the algorithm, as it shows the connections available between two nodes.
 * 
 */
class Edge {
public:
    /**
     * @brief The node from where the Edge will start. 
     * 
     */
    std::shared_ptr<Node> fromNode;
    /**
     * @brief The node where the Edge will end up.
     * 
     */
    std::shared_ptr<Node> toNode;

    /**
     * @brief Construct a new Edge object with no specified m_fromNode and m_toNode, constructs them in place with default values.
     * 
     * @see m_fromNode
     * @see m_toNode
     */
    Edge()
        : fromNode(std::shared_ptr<Node>())
        , toNode(std::shared_ptr<Node>())
    {
    }

    /**
     * @brief Construct a new Edge object with the given parameters: fromNode, toNode.
     * 
     * @param fromNode The node from where the Edge will start.
     * @param toNode The node where the Edge will end up.
     * 
     * @see fromNode
     * @see toNode
     */
    Edge(const std::shared_ptr<Node> from, const std::shared_ptr<Node> to)
        : fromNode(from)
        , toNode(to)
    {
    }
};
#endif
