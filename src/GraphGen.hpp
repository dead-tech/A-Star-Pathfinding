#ifndef GRAPH_GEN_H
#define GRAPH_GEN_H

// Global includes
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <time.h>
#include <memory>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

// My headers
#include "Node.hpp"
#include "Edge.hpp"


class Graph {
public:
    Graph(const std::size_t graphWidth);

    [[nodiscard]] const std::unordered_map<Node*, std::vector<Edge>>& getGraph() const { return m_graphRepr; }

    [[nodiscard]] Node* findNode(float x, float y) const;

    void setStartNode(float x, float y) const;

    void setEndNode(float x, float y) const;


    [[nodiscard]] Node& operator[](unsigned int index)
    {
        return m_graphNodes.at(index);
    }

private:
    std::unordered_map<Node*, std::vector<Edge>> m_graphRepr;
    std::vector<Node>                            m_graphNodes;

    const double m_createEdgeTreshold = 0.5;
};
#endif
