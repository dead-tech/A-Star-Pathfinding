#ifndef GRAPH_GEN_H
#define GRAPH_GEN_H

// Global includes
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <time.h>


// My headers
#include "Node.hpp"
#include "Edge.hpp"


class Graph {
public:
    Graph(const std::size_t graphWidth);

    [[nodiscard]] const std::unordered_map<Node*, std::vector<Edge>> getGraph() const { return m_graphRepr; }

private:
    std::unordered_map<Node*, std::vector<Edge>> m_graphRepr;
    std::vector<Node>                            m_graphNodes;

    const double m_createEdgeTreshold = 0.5;
};
#endif
