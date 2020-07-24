#include "GraphGen.hpp"

Graph::Graph(const std::size_t graphWidth)
{
    m_graphNodes = std::vector<Node>();
    m_graphNodes.reserve(graphWidth * graphWidth);

    for (std::size_t i = 0; i < graphWidth; ++i) {
        for (std::size_t j = 0; j < graphWidth; ++j) {

            const Vector<double, 2> nodeCoords = { static_cast<double>(i), static_cast<double>(j) };
            const Node              node { nodeCoords, true };

            m_graphNodes.push_back(node);
        }
    }

    srand(static_cast<unsigned int>(time(NULL)));

    m_graphRepr = std::unordered_map<Node*, std::vector<Edge>>();

    std::vector<Node> neighbours;
    std::vector<Edge> edges;

    for (std::size_t i = 0; i < m_graphNodes.size(); i++) {
        auto createEdges = [&](const auto node) {
            const double randValue = static_cast<double>(rand()) / RAND_MAX;
            if (randValue > m_createEdgeTreshold) {
                edges.push_back(Edge(m_graphNodes[i], node));
            }
        };

        auto findNeighbours = [&](const auto node) {
            if (((std::abs(node.X() - m_graphNodes[i].X()) == 1 && node.Y() == m_graphNodes[i].Y()) || (std::abs(node.Y() - m_graphNodes[i].Y()) == 1 && node.X() == m_graphNodes[i].X())) && !(node == m_graphNodes[i])) {
                neighbours.push_back(node);
            }
        };

        std::for_each(begin(m_graphNodes), end(m_graphNodes), findNeighbours);

        std::for_each(begin(neighbours), end(neighbours), createEdges);

        m_graphRepr.insert({ &m_graphNodes[i], edges });

        neighbours.clear();
        edges.clear();
    }
}

__NODISCARD Node* Graph::findNode(float x, float y) const
{
    for (auto [node, edges] : m_graphRepr) {
        const auto resultX = std::abs(x - node->X());
        const auto resultY = std::abs(y - node->Y());

        if ((resultX >= 0 && resultX < 0.25) && (resultY >= 0 && resultY < 0.25)) {
            return node;
        }
    }

    return nullptr;
}

void Graph::setStartNode(float x, float y) const
{

    auto isStartNode = [&](const std::pair<Node*, std::vector<Edge>> pair) {
        return pair.first->m_isStartNode;
    };

    const auto match = std::find_if(begin(m_graphRepr), end(m_graphRepr), isStartNode);

    if (match != m_graphRepr.end()) {
        match->first->m_isStartNode = false;
    }

    auto node = findNode(x, y);
    if (node != nullptr) {
        node->m_isEndNode   = false;
        node->m_isStartNode = true;
    }
}

void Graph::setEndNode(float x, float y) const
{
    auto isEndNode = [&](const std::pair<Node*, std::vector<Edge>> pair) {
        return pair.first->m_isEndNode;
    };

    const auto match = std::find_if(begin(m_graphRepr), end(m_graphRepr), isEndNode);

    if (match != m_graphRepr.end()) {
        match->first->m_isEndNode = false;
    }

    auto node = findNode(x, y);
    if (node != nullptr) {
        node->m_isStartNode = false;
        node->m_isEndNode   = true;
    }
}
