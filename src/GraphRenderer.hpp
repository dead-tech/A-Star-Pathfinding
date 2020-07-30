#ifndef GRAPH_RENDERER_H
#define GRAPH_RENDERER_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <array>
#include <iterator>
#include <algorithm>

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

    /** 
     * Line struct.
     * Contains coordinates for creating a line between two nodes.
     */
    struct Line {
        double startX; /**< Starting point on the x axis. */
        double startY; /**< Starting point on the y axis. */
        double endX;   /**< Ending point on the x axis. */
        double endY;   /**< Ending point on the y axis. */

        Line(Vector<double, 2> v1, Vector<double, 2> v2)
        {
            startX = v1.x;
            endX   = v2.x;
            startY = v1.y;
            endY   = v2.y;
        }
    };

    std::vector<Vector<double, 2>> m_result;

public:
    GLFWwindow*            m_window;
    std::unique_ptr<Graph> m_graph;


    GraphRenderer(std::unique_ptr<Graph> graph);
    void cleanup();
    void drawTriangle(const std::array<double, 9>& vertices) const;
    void handleInput();
    void draw();
    void drawNode(const Vector<double, 2> coords);
    void drawNode(const Vector<double, 2> coords, const float scale);
    /**
       * Draws a line between two nodes, whose coordinates are specified in the Line struct.
       * @param line struct containing coordinates.
       * @see Line
       * @return void
    */
    void                           drawLine(const Line line);
    const glm::vec2                rayCastCoords() const;
    glm::dvec4                     toWorldCoords(glm::dvec4 ndcCoords) const;
    std::vector<Vector<double, 2>> reconstructPath(std::unordered_map<Vector<double, 2>, Vector<double, 2>> path);
    std::vector<Vector<double, 2>> aStar();
};

#endif
