#include "GraphGen.hpp"

Graph::Graph(const std::size_t graphWidth)
{
    m_graphNodes = std::unordered_map<Vector<double, 2>, std::shared_ptr<Node>>();

    for (std::size_t i = 0; i < graphWidth; ++i) {
        for (std::size_t j = 0; j < graphWidth; ++j) {
            double x = static_cast<double>(i);
            double y = static_cast<double>(j);
            auto   p = std::make_shared<Node>(std::move(Node { { x, y }, true }));
            m_graphNodes.emplace(Vector<double, 2> { x, y }, std::move(p));
        }
    }

    std::random_device rd;

    std::mt19937                           generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    m_graphRepr = std::unordered_map<std::shared_ptr<Node>, std::vector<Edge>>();

    std::vector<Edge> edges;

    auto createEdgesFor = [&](const auto node) {
        for (const auto coord : neighbours(node)) {
            if (distribution(generator) > m_createEdgeThreshold) {
                if (m_graphNodes.contains(coord))
                    edges.push_back(Edge(node, m_graphNodes[coord]));
            }
        }
    };

    for (const auto [coords, node] : m_graphNodes) {
        createEdgesFor(node);
        m_graphRepr.emplace(node, edges); // ?
        edges.clear();
    }
}

__NODISCARD const std::vector<Vector<double, 2>> Graph::neighbours(const std::shared_ptr<Node> node) const
{
    std::vector<Vector<double, 2>> neighbours;

    const auto nodeCoords = node->getCoords();

    neighbours.push_back({ nodeCoords.x + 1.0, nodeCoords.y });
    neighbours.push_back({ nodeCoords.x, nodeCoords.y + 1.0 });
    neighbours.push_back({ nodeCoords.x - 1.0, nodeCoords.y });
    neighbours.push_back({ nodeCoords.x, nodeCoords.y - 1.0 });

    return neighbours;
}
