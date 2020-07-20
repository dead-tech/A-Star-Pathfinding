#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "GraphGen.hpp"
#include "GraphRenderer.hpp"

int main()
{
    const std::size_t graphDim = 10;
    Graph             graph { graphDim };
    GraphRenderer     renderer {};


    while (!glfwWindowShouldClose(renderer.m_window)) {
        renderer.handleInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.draw(graph.getGraph());

        glfwSwapBuffers(renderer.m_window);
        glfwPollEvents();
    }

    return 0;
}
