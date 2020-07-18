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
    int         m_windowWidth  = 1400;
    int         m_windowHeight = 900;
    const char* m_windowTitle  = "A Star Pathfinding";

    unsigned int m_vertexArrayObj;
    unsigned int m_vertexBufferObj;

    unsigned int m_defaultShader;

    const std::vector<double> m_offsets {
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
    void drawTriangle(std::vector<double>& vertices);
    void handleInput();
    void draw(std::unordered_map<Node*, std::vector<Edge>> nodes);
    void drawNode(double x, double y);
    void drawLine(double startX, double startY, double endX, double endY);
};

#endif
