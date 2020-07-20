#include "GraphGen.hpp"

Graph::Graph(const std::size_t graphWidth)
{
    m_graphNodes = std::vector<Node>();
    m_graphNodes.reserve(graphWidth * graphWidth);

    for (uint32_t i = 0; i < graphWidth; i++) {
        for (uint32_t j = 0; j < graphWidth; j++) {

            const Vector<double, 2> nodeCoords = { static_cast<double>(i), static_cast<double>(j) };
            const Node              node(nodeCoords, true);

            m_graphNodes.push_back(node);
        }
    }

    srand(time(NULL));

    m_graphRepr = std::unordered_map<Node*, std::vector<Edge>>();

    std::vector<Node> neighbours;
    std::vector<Edge> edges;

    for (uint32_t i = 0; i < m_graphNodes.size(); i++) {
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

Node* Graph::findNode(float x, float y) const
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
