#ifndef GRAPH_GEN_H

#define GRAPH_GEN_H
#define __NODISCARD [[nodiscard]]

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <time.h>
#include <memory>
#include <unordered_set>
#include <random>

#include "Node.hpp"
#include "Edge.hpp"


class Graph {
public:
    std::shared_ptr<Node> startNode = nullptr;
    std::shared_ptr<Node> endNode   = nullptr;

    Graph(const std::size_t graphWidth);

    __NODISCARD const std::unordered_map<std::shared_ptr<Node>, std::vector<Edge>>& getGraph() const { return m_graphRepr; }

    __NODISCARD const bool contains(Vector<double, 2> coords) const
    {
        return m_graphNodes.contains(coords);
    }
    //__NODISCARD std::unique_ptr<Node> findNode(float x, float y) const;

    //void setStartNode(float x, float y) const;

    //void setEndNode(float x, float y) const;


    __NODISCARD const std::shared_ptr<Node>& operator[](const Vector<double, 2>& coords) const
    {
        return m_graphNodes.at(coords);
    }

    __NODISCARD const std::vector<Vector<double, 2>> neighbours(const std::shared_ptr<Node> node) const;


private:
    //std::unordered_map<Node*, std::vector<Edge>> m_graphRepr;
    std::unordered_map<std::shared_ptr<Node>, std::vector<Edge>> m_graphRepr;
    std::unordered_map<Vector<double, 2>, std::shared_ptr<Node>> m_graphNodes;

    const double m_createEdgeThreshold = 0.3;
};
#endif
