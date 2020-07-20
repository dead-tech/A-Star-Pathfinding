#include "GraphRenderer.hpp"


GraphRenderer::GraphRenderer()
{
    if (!glfwInit()) {
        fmt::print("glfw3 failed to initialize!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle, NULL, NULL);

    if (m_window == NULL) {
        fmt::print("Failed to create GLFW window, {}, {}");
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fmt::print("Glad failed to initialize!");
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight); // Starts from bottom left corner.

    glfwSetFramebufferSizeCallback(m_window, (GLFWframebuffersizefun)framebufferSizeCallback); // Sets a callback function for resizing the window.


    glGenVertexArrays(1, &m_vertexArrayObj);
    glGenBuffers(1, &m_vertexBufferObj);

    Shader vertexShader { "assets/shaders/vertexShader.glsl" };
    auto   vertexShaderID = vertexShader.compileShader(true);

    Shader fragmentShader { "assets/shaders/fragmentShader.glsl" };
    auto   fragmentShaderID = fragmentShader.compileShader(false);

    m_defaultShader = glCreateProgram();
    glAttachShader(m_defaultShader, vertexShaderID);
    glAttachShader(m_defaultShader, fragmentShaderID);
    glLinkProgram(m_defaultShader);
}

void GraphRenderer::handleInput() const
{
    // If Escape Key is pressed
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void GraphRenderer::drawTriangle(const std::array<double, 9>& vertices) const
{

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

void GraphRenderer::draw(const std::unordered_map<Node*, std::vector<Edge>> nodes)
{

    //m_scaleMatrix    = glm::mat4(1.0f);
    //auto scaleFactor = 1.0f;
    //m_scaleMatrix    = glm::scale(m_scaleMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    //m_scaleMatrix    = glm::translate(m_scaleMatrix, glm::vec3(-4.5f, -4.5f, 0.0f));

    ////Shader::setUniformMat4("scaleTransform", m_defaultShader, m_scaleMatrix);


    const auto getCoordinates = [&](const auto node) {
        return std::make_tuple<double>(node->X(), node->Y());
    };

    for (const auto& [node, edges] : nodes) {
        const auto [x, y] = getCoordinates(node);
        drawNode(x, y);
        for (const auto& edge : edges) {

            const auto getFromNode = edge.getFromNode();
            const auto getToNode   = edge.getToNode();

            const auto [sx, sy] = getCoordinates(&getFromNode);
            const auto [fx, fy] = getCoordinates(&getToNode);

            drawLine(sx, sy, fx, fy);
        }
    }
}

void GraphRenderer::drawNode(const double x, const double y)
{
    auto scaleFactor = 1;

    m_model = glm::mat4(1.0f);
    m_model = glm::scale(m_model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));

    m_view = glm::lookAt(glm::vec3(4.5f, 4.5f, 15.0f), glm::vec3(4.5f, 4.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


    Shader::setUniformMat4("model", m_defaultShader, m_model);
    Shader::setUniformMat4("view", m_defaultShader, m_view);
    Shader::setUniformMat4("proj", m_defaultShader, m_proj);

    drawTriangle(m_firstTriangle);
    drawTriangle(m_secondTriangle);
}

void GraphRenderer::drawLine(const double startX, const double startY, const double endX, const double endY) const
{

    const std::array<double, 6> vertices = { startX, startY, 0.0, endX, endY, 0.0 };

    glBindVertexArray(m_vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObj);

    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(double) * 3, (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto model = glm::mat4(1.0f);

    Shader::setUniformMat4("model", m_defaultShader, model);

    glUseProgram(m_defaultShader);
    glBindVertexArray(m_vertexArrayObj);
    glDrawArrays(GL_LINES, 0, 2);
}

glm::vec4 GraphRenderer::toWorldCoords(glm::vec4 ndcCoords)
{

    auto eye = glm::inverse(m_proj) * ndcCoords;
    eye      = glm::vec4(eye.x, eye.y, -1.0f, 0.0f);

    return glm::inverse(m_view) * eye;

    //return glm::inverse(m_proj * m_view) * ndcCoords;
    //return glm::unProject(glm::vec3(ndcCoords), m_view, m_proj, glm::vec4(0, 0, m_windowWidth, m_windowHeight));
}

GraphRenderer::~GraphRenderer()
{
    glDeleteVertexArrays(1, &m_vertexArrayObj);
    glDeleteBuffers(1, &m_vertexBufferObj);
    glDeleteProgram(m_defaultShader);
    glfwTerminate();
}
