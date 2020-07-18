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

        //Render here

        if (glfwGetMouseButton(renderer.m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(renderer.m_window, &xpos, &ypos);
            double scaledX, scaledY;
            scaledX = (2.0f * xpos) / 800 - 1.0f;
            scaledY = 1.0f - (2.0f * ypos) / 800;

            fmt::print("Scaled x: {}, scaled y: {}\n", scaledX, scaledY);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.draw(graph.getGraph());

        glfwSwapBuffers(renderer.m_window);
        glfwPollEvents();
    }

    return 0;
}
