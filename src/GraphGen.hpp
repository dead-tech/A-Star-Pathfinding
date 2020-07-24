#ifndef GRAPH_GEN_H

#define GRAPH_GEN_H
#define __NODISCARD [[nodiscard]]

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <time.h>
#include <memory>

#include "Node.hpp"
#include "Edge.hpp"


class Graph {
public:
    Graph(const std::size_t graphWidth);

    __NODISCARD const std::unordered_map<Node*, std::vector<Edge>>& getGraph() const { return m_graphRepr; }

    __NODISCARD Node* findNode(float x, float y) const;

    void setStartNode(float x, float y) const;

    void setEndNode(float x, float y) const;


    __NODISCARD Node& operator[](unsigned int index)
    {
        return m_graphNodes.at(index);
    }

private:
    std::unordered_map<Node*, std::vector<Edge>> m_graphRepr;
    std::vector<Node>                            m_graphNodes;

    const double m_createEdgeTreshold = 0.5;
};
#endif
