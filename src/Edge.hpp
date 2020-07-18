#ifndef EDGE_H
#define EDGE_H
#include "Node.hpp"

class Edge {
public:
    Edge()
        : m_fromNode(Node())
        , m_toNode(Node())
    {
    }

    Edge(const Node fromNode, const Node toNode)
        : m_fromNode(fromNode)
        , m_toNode(toNode)
    {
    }

    [[nodiscard]] const Node getFromNode() const { return m_fromNode; }
    [[nodiscard]] const Node getToNode() const { return m_toNode; }

private:
    Node m_fromNode;
    Node m_toNode;
};
#endif
