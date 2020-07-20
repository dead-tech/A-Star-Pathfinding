#ifndef GRAPH_RENDERER_H
#define GRAPH_RENDERER_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Shader.hpp"
#include "Edge.hpp"
#include "Node.hpp"
#include "GraphGen.hpp"

class GraphRenderer {
private:
    const int   m_windowWidth  = 800;
    const int   m_windowHeight = 800;
    const char* m_windowTitle  = "A Star Pathfinding";

    unsigned int m_vertexArrayObj;
    unsigned int m_vertexBufferObj;

    unsigned int m_defaultShader;

    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_proj;


    const std::array<double, 9> m_firstTriangle {
        -0.25f, -0.25f, 0.0f, //a
        -0.25f, 0.25f, 0.0f,  //b
        0.25f, 0.25f, 0.0f,   //c
    };

    const std::array<double, 9> m_secondTriangle {
        0.25f, 0.25f, 0.0f,   //c
        0.25f, -0.25f, 0.0f,  //d
        -0.25f, -0.25f, 0.0f, //a
    };

    glm::mat4 m_scaleMatrix = glm::mat4(1.0f);

    static void framebufferSizeCallback(int width, int height)
    {
        glViewport(0, 0, width, height);
    }


public:
    GLFWwindow* m_window;
    Graph*      m_graph;

    GraphRenderer(Graph* graph);
    ~GraphRenderer();
    void      drawTriangle(const std::array<double, 9>& vertices) const;
    void      handleInput();
    void      draw();
    void      drawNode(const double x, const double y);
    void      drawLine(const double startX, const double startY, const double endX, const double endY) const;
    glm::vec4 toWorldCoords(glm::vec4 ndcCoords) const;
};

#endif
