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

        /*if (glfwGetMouseButton(renderer.m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(renderer.m_window, &xpos, &ypos);
            glm::vec4 vec(xpos - 700, ypos - 450, 0, 0);
            glm::vec4 vec(0, 0, 0, 0);
            glm::mat4 trans = glm::mat4(1.0f);
            trans           = glm::scale(trans, glm::vec3((double)(1 / 700.0), (double)(1 / 450.0), 1.0f));
            vec             = trans * vec;
            fmt::print("X:{}, Y:{}\n", vec.x, vec.y);
            glm::mat4 view = glm::mat4(1.0f);
            view           = glm::translate(view, glm::vec3(5.0f, 5.0f, 0.0f));
        }*/

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.draw(graph.getGraph());

        glfwSwapBuffers(renderer.m_window);
        glfwPollEvents();
    }

    return 0;
}
