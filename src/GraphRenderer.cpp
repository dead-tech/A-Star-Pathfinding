#include "GraphRenderer.hpp"


GraphRenderer::GraphRenderer(std::unique_ptr<Graph> graph)
    : m_graph { std::move(graph) }
{
    if (!glfwInit()) {
        fmt::print("Glfw3 failed to initialize!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle, NULL, NULL);

    if (m_window == NULL) {
        const char*  errorMessage;
        unsigned int errorCode = glfwGetError(&errorMessage);
        fmt::print("Failed to create GLFW window! Error Code: {}, Error Message: {}", errorCode, errorMessage);
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fmt::print("Glad failed to initialize!");
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight); // Starts from bottom left corner.

    glGenVertexArrays(1, &m_vertexArrayObj);
    glGenBuffers(1, &m_vertexBufferObj);

    Shader     vertexShader { "assets/shaders/vertexShader.vert" };
    const auto vertexShaderID { vertexShader.compileShader(true) };

    Shader     fragmentShader { "assets/shaders/fragmentShader.frag" };
    const auto fragmentShaderID { fragmentShader.compileShader(false) };

    m_defaultShader = glCreateProgram();
    glAttachShader(m_defaultShader, vertexShaderID);
    glAttachShader(m_defaultShader, fragmentShaderID);
    glLinkProgram(m_defaultShader);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void GraphRenderer::handleInput()
{

    // If Escape Key is pressed
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

        glfwSetWindowShouldClose(m_window, true);
    }

    // If Escape Enter is pressed
    if (glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {

        m_result = aStar();
    }

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

        auto graphCoords = rayCastCoords();

        graphCoords.x = std::abs(std::floor(graphCoords.x) - graphCoords.x) < 0.5 ? std::floor(graphCoords.x) : std::ceil(graphCoords.x);
        graphCoords.y = std::abs(std::floor(graphCoords.y) - graphCoords.y) < 0.5 ? std::floor(graphCoords.y) : std::ceil(graphCoords.y);

        Vector<double, 2> coords { graphCoords.x, graphCoords.y };

        if (m_graph->contains(coords)) {
            m_graph->startNode = std::shared_ptr<Node>(m_graph->operator[](coords));
        }

    } else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {

        auto graphCoords = rayCastCoords();

        graphCoords.x = std::abs(std::floor(graphCoords.x) - graphCoords.x) < 0.5 ? std::floor(graphCoords.x) : std::ceil(graphCoords.x);
        graphCoords.y = std::abs(std::floor(graphCoords.y) - graphCoords.y) < 0.5 ? std::floor(graphCoords.y) : std::ceil(graphCoords.y);

        Vector<double, 2> coords { graphCoords.x, graphCoords.y };

        if (m_graph->contains(coords)) {
            m_graph->endNode = std::shared_ptr<Node>(m_graph->operator[](coords));
        }
    }
}

void GraphRenderer::drawTriangle(const std::array<double, 9>& vertices) const
{

    if (vertices.size() != 9) {
        fmt::print("Not enough vertices in the array to draw a triangle!");
        return;
    }

    glBindVertexArray(m_vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObj);

    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(double) * 3, (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(m_defaultShader);
    glBindVertexArray(m_vertexArrayObj);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GraphRenderer::draw()
{

    for (const auto& [node, edges] : m_graph->getGraph()) {

        Shader::setUniformBool("path", m_defaultShader, false);
        Shader::setUniformBool("endNode", m_defaultShader, false);
        Shader::setUniformBool("startNode", m_defaultShader, false);


        if (m_graph->startNode != nullptr && *node == *m_graph->startNode) {

            Shader::setUniformBool("path", m_defaultShader, false);
            Shader::setUniformBool("endNode", m_defaultShader, false);
            Shader::setUniformBool("startNode", m_defaultShader, true);
        }

        if (m_graph->endNode != nullptr && *node == *m_graph->endNode) {

            Shader::setUniformBool("path", m_defaultShader, false);
            Shader::setUniformBool("startNode", m_defaultShader, false);
            Shader::setUniformBool("endNode", m_defaultShader, true);
        }

        // clang-format off
        auto match = std::find_if(m_result.begin(), m_result.end(), [&](const auto r) {
            return r == node->getCoords() 
                && node->getCoords() != m_graph->endNode->getCoords() 
                && node->getCoords() != m_graph->startNode->getCoords();
        });
        // clang-format on

        if (match != m_result.end()) {
            Shader::setUniformBool("endNode", m_defaultShader, false);
            Shader::setUniformBool("startNode", m_defaultShader, false);
            Shader::setUniformBool("path", m_defaultShader, true);
        }

        drawNode(node->getCoords());

        for (const auto& edge : edges) {
            const Line line { edge.fromNode->getCoords(), edge.toNode->getCoords() };
            drawLine(line);
        }
    }
}

void GraphRenderer::drawNode(const Vector<double, 2> coords)
{

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, glm::vec3(coords.x, coords.y, 0.0f));

    m_view = glm::lookAt(glm::vec3(4.5f, 4.5f, 15.0f), glm::vec3(4.5f, 4.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


    Shader::setUniformMat4("model", m_defaultShader, m_model);
    Shader::setUniformMat4("view", m_defaultShader, m_view);
    Shader::setUniformMat4("proj", m_defaultShader, m_proj);

    drawTriangle(m_firstTriangle);
    drawTriangle(m_secondTriangle);
}

void GraphRenderer::drawNode(const Vector<double, 2> coords, const float scale)
{

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, glm::vec3(coords.x, coords.y, 0.0f));
    m_model = glm::scale(m_model, glm::vec3(scale, scale, 1.0f));

    m_view = glm::lookAt(glm::vec3(4.5f, 4.5f, 15.0f), glm::vec3(4.5f, 4.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


    Shader::setUniformMat4("model", m_defaultShader, m_model);
    Shader::setUniformMat4("view", m_defaultShader, m_view);
    Shader::setUniformMat4("proj", m_defaultShader, m_proj);

    drawTriangle(m_firstTriangle);
    drawTriangle(m_secondTriangle);
}

void GraphRenderer::drawLine(const Line line)
{

    const std::array<double, 6> vertices = { line.startX, line.startY, 0.0, line.endX, line.endY, 0.0 };


    drawNode(Vector<double, 2> { line.endX, line.endY }, 0.10f);

    glBindVertexArray(m_vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObj);

    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(double) * 3, (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const auto model = glm::mat4(1.0f);

    Shader::setUniformMat4("model", m_defaultShader, model);

    glUseProgram(m_defaultShader);
    glBindVertexArray(m_vertexArrayObj);
    glDrawArrays(GL_LINES, 0, 2);
}

const glm::vec2 GraphRenderer::rayCastCoords() const
{
    double mouseX;
    double mouseY;


    glfwGetCursorPos(m_window, &mouseX, &mouseY);


    double ndcX;
    double ndcY;
    ndcX           = (2.0 * mouseX) / 800.0 - 1.0;
    ndcY           = 1.0 - (2.0 * mouseY) / 800.0;
    glm::dvec4 ndc = glm::dvec4(ndcX, ndcY, -1.0, 1.0);

    auto worldCoords = glm::dvec3(toWorldCoords(ndc));
    worldCoords      = glm::normalize(worldCoords);

    double t = -15.0 / worldCoords.z;

    double finalX = 4.5 + t * worldCoords.x;
    double finalY = 4.5 + t * worldCoords.y;


    return glm::dvec2(finalX, finalY);
}

glm::dvec4 GraphRenderer::toWorldCoords(glm::dvec4 ndcCoords) const
{

    auto eye = glm::inverse(m_proj) * ndcCoords;
    eye      = glm::dvec4(eye.x, eye.y, -1.0f, 0.0f);

    return glm::inverse(m_view) * eye;
}

std::vector<Vector<double, 2>> GraphRenderer::reconstructPath(std::unordered_map<Vector<double, 2>, Vector<double, 2>> path)
{
    std::vector<Vector<double, 2>> resultPath { m_graph->endNode->getCoords() };

    auto current = m_graph->endNode->getCoords();

    while (path.contains(current)) {
        current = path[current];
        resultPath.push_back(current);
    }
    return resultPath;
}

std::vector<Vector<double, 2>> GraphRenderer::aStar()
{
    auto distance = [&](std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
        auto c1 = n1->getCoords();
        auto c2 = n2->getCoords();

        return std::abs(c1.x - c2.x) + std::abs(c2.y - c1.y);
    };

    auto heuristic = [&](std::shared_ptr<Node> currentNode) {
        return distance(currentNode, m_graph->endNode);
    };

    std::unordered_set<Vector<double, 2>> openSet = { m_graph->startNode->getCoords() };

    std::unordered_map<Vector<double, 2>, Vector<double, 2>> cameFrom {};

    std::unordered_map<Vector<double, 2>, int> gScore({});

    std::unordered_map<Vector<double, 2>, int> fScore({});

    // for_each
    for (const auto& [node, edges] : m_graph->getGraph()) {
        if (*node != *m_graph->startNode) {
            gScore.emplace(node->getCoords(), 500);
            fScore.emplace(node->getCoords(), 500);
        } else {
            gScore.emplace(node->getCoords(), 0);
            fScore.emplace(node->getCoords(), heuristic(node));
        }
    }

    while (!openSet.empty()) {
        // Create appropriate lambda
        auto min = *(std::min_element(openSet.begin(), openSet.end(), [&](const auto a, const auto b) {
            return fScore[a] < fScore[b];
        }));

        if (min == m_graph->endNode->getCoords()) {
            return reconstructPath(cameFrom); // Reconstruct Path;
        }

        openSet.erase(min); // ?
        const auto node = m_graph->operator[](min);

        auto container = m_graph->getGraph();

        for (const auto& edge : container[node]) {
            auto       neighbor         = edge.toNode;
            auto       n_coords         = neighbor->getCoords();
            const auto tentative_gScore = gScore[min] + distance(node, neighbor);
            if (tentative_gScore < gScore[neighbor->getCoords()]) {
                cameFrom[n_coords] = min;
                gScore[n_coords]   = tentative_gScore;
                fScore[n_coords]   = gScore[n_coords] + heuristic(neighbor);

                if (!openSet.contains(n_coords)) {
                    openSet.emplace(n_coords);
                }
            }
        }
    }
    return std::vector<Vector<double, 2>>();
}

void GraphRenderer::cleanup()
{
    glDeleteVertexArrays(1, &m_vertexArrayObj);
    glDeleteBuffers(1, &m_vertexBufferObj);
    glDeleteProgram(m_defaultShader);
    glfwTerminate();
}
