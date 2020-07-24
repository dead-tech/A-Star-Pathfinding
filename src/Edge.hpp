#ifndef EDGE_H
#define EDGE_H
#include "Node.hpp"

#define __NODISCARD [[nodiscard]]

class Edge {
public:
    Edge()
        : m_fromNode(Node {})
        , m_toNode(Node {})
    {
    }

    Edge(const Node fromNode, const Node toNode)
        : m_fromNode { fromNode }
        , m_toNode { toNode }
    {
    }

    __NODISCARD const Node getFromNode() const { return m_fromNode; }
    __NODISCARD const Node getToNode() const { return m_toNode; }

private:
    Node m_fromNode;
    Node m_toNode;
};
#endif
