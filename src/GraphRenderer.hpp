#ifndef GRAPH_RENDERER_H
#define GRAPH_RENDERER_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Edge.hpp"
#include "Node.hpp"

class GraphRenderer {
private:
    const int   m_windowWidth  = 800;
    const int   m_windowHeight = 800;
    const char* m_windowTitle  = "A Star Pathfinding";

    unsigned int m_vertexArrayObj;
    unsigned int m_vertexBufferObj;

    unsigned int m_defaultShader;

    const std::array<double, 15> m_offsets {
        -0.25f, -0.25f, 0.0f, //a
        -0.25f, 0.25f, 0.0f,  //b
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
    GraphRenderer();
    ~GraphRenderer();
    void drawTriangle(const std::vector<double>& vertices) const;
    void handleInput() const;
    void draw(const std::unordered_map<Node*, std::vector<Edge>> nodes);
    void drawNode(const double x, const double y) const;
    void drawLine(const double startX, const double startY, const double endX, const double endY) const;
};

#endif
