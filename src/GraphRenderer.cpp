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

    } else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

        auto coords = rayCastCoords();
        m_graph->setStartNode(coords.x, coords.y);

    } else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {

        auto coords = rayCastCoords();
        m_graph->setEndNode(coords.x, coords.y);
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
    const auto getCoordinates = [&](const auto node) {
        return std::make_tuple<double>(node->X(), node->Y());
    };

    for (const auto& [node, edges] : m_graph->getGraph()) {
        const auto [_x, _y] = getCoordinates(node);

        const Point point { .x = _x, .y = _y };

        if (node->m_isStartNode) {

            Shader::setUniformBool("endNode", m_defaultShader, false);
            Shader::setUniformBool("startNode", m_defaultShader, true);

            drawNode(point);

            Shader::setUniformBool("startNode", m_defaultShader, false);


        } else if (node->m_isEndNode) {

            Shader::setUniformBool("startNode", m_defaultShader, false);
            Shader::setUniformBool("endNode", m_defaultShader, true);

            drawNode(point);

            Shader::setUniformBool("endNode", m_defaultShader, false);


        } else {

            Shader::setUniformBool("endNode", m_defaultShader, false);
            Shader::setUniformBool("startNode", m_defaultShader, false);

            drawNode(point);
        }

        for (const auto& edge : edges) {

            const auto getFromNode { edge.getFromNode() };
            const auto getToNode { edge.getToNode() };

            const auto [fx, fy] = getCoordinates(&getFromNode);
            const auto [tx, ty] = getCoordinates(&getToNode);

            const Line line { .startX = fx, .startY = fy, .endX = tx, .endY = ty };

            drawLine(line);
        }
    }
}

void GraphRenderer::drawNode(const Point point)
{

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, glm::vec3(point.x, point.y, 0.0f));

    m_view = glm::lookAt(glm::vec3(4.5f, 4.5f, 15.0f), glm::vec3(4.5f, 4.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


    Shader::setUniformMat4("model", m_defaultShader, m_model);
    Shader::setUniformMat4("view", m_defaultShader, m_view);
    Shader::setUniformMat4("proj", m_defaultShader, m_proj);

    drawTriangle(m_firstTriangle);
    drawTriangle(m_secondTriangle);
}

void GraphRenderer::drawLine(const Line line) const
{

    const std::array<double, 6> vertices = { line.startX, line.startY, 0.0, line.endX, line.endY, 0.0 };

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

    float ndcX;
    float ndcY;
    ndcX          = (2.0f * static_cast<float>(mouseX)) / 800 - 1.0f;
    ndcY          = 1.0f - (2.0f * static_cast<float>(mouseY)) / 800;
    glm::vec4 ndc = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

    auto worldCoords = glm::vec3(toWorldCoords(ndc));
    worldCoords      = glm::normalize(worldCoords);

    auto t = -15.0f / worldCoords.z;

    auto finalX = 4.5f + t * worldCoords.x;
    auto finalY = 4.5f + t * worldCoords.y;

    return glm::vec2(finalX, finalY);
}

glm::vec4 GraphRenderer::toWorldCoords(glm::vec4 ndcCoords) const
{

    auto eye = glm::inverse(m_proj) * ndcCoords;
    eye      = glm::vec4(eye.x, eye.y, -1.0f, 0.0f);

    return glm::inverse(m_view) * eye;
}

void GraphRenderer::cleanup()
{
    glDeleteVertexArrays(1, &m_vertexArrayObj);
    glDeleteBuffers(1, &m_vertexBufferObj);
    glDeleteProgram(m_defaultShader);
    glfwTerminate();
}
