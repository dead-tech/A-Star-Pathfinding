#ifndef EDGE_H
#define EDGE_H
#include "Node.hpp"

#define __NODISCARD [[nodiscard]]

/**
 * @brief This Edge class helps with the implementation of the algorithm, as it shows the connections available between two nodes.
 * 
 */
class Edge {
public:
    /**
     * @brief Construct a new Edge object with no specified m_fromNode and m_toNode, constructs them in place with default values.
     * 
     * @see m_fromNode
     * @see m_toNode
     */
    Edge()
        : m_fromNode(Node {})
        , m_toNode(Node {})
    {
    }

    /**
     * @brief Construct a new Edge object with the given parameters: fromNode, toNode.
     * 
     * @param fromNode The node from where the Edge will start.
     * @param toNode The node where the Edge will end up.
     * 
     * @see m_fromNode
     * @see m_toNode
     */
    Edge(const Node fromNode, const Node toNode)
        : m_fromNode { fromNode }
        , m_toNode { toNode }
    {
    }

    /**
     * @brief Get the start node of the edge.
     * 
     * @see m_fromNode.
     * @return m_fromNode.
     */
    __NODISCARD const Node getFromNode() const { return m_fromNode; }
    /**
     * @brief Get the end node of the edge.
     * 
     * @see m_toNode.
     * @return m_toNode.
     */
    __NODISCARD const Node getToNode() const { return m_toNode; }

private:
    /**
     * @brief The node from where the Edge will start. 
     * 
     */
    Node m_fromNode;
    /**
     * @brief The node where the Edge will end up.
     * 
     */
    Node m_toNode;
};
#endif
