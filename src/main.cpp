#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "GraphGen.hpp"
#include "GraphRenderer.hpp"

int main()
{
    const std::size_t graphDim = 10;
    Graph             graph { graphDim };
    GraphRenderer     renderer {};

    auto printVec = [&](const auto v, const char* s) {
        fmt::print("{} -> X: {}, Y:{}, Z:{}, W:{}\n", s, v.x, v.y, v.z, v.w);
    };

    auto printVec3 = [&](const auto v, const char* s) {
        fmt::print("{} -> X: {}, Y:{}, Z:{}\n", s, v.x, v.y, v.z);
    };


    while (!glfwWindowShouldClose(renderer.m_window)) {
        renderer.handleInput();

        //Render here

        if (glfwGetMouseButton(renderer.m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            double xpos, ypos;
            //fmt::print("x: {}, y: {}\n", xpos, ypos);
            glfwGetCursorPos(renderer.m_window, &xpos, &ypos);
            double scaledX, scaledY;
            scaledX = (2.0f * xpos) / 800 - 1.0f;
            scaledY = 1.0f - (2.0f * ypos) / 800;

            //glm::vec4 ndc      = glm::vec4(scaledX, scaledY, 1.0f, 0.0f);
            glm::vec4 ndc    = glm::vec4(scaledX, scaledY, -1.0f, 1.0f);
            auto      result = glm::vec3(renderer.toWorldCoords(ndc));
            result           = glm::normalize(result);
            auto t           = -15.0f / result.z;
            fmt::print("{}\n", result.z);
            //printVec3(result, "Result: ");

            auto pX = 4.5f + t * result.x;
            auto pY = 4.5f + t * result.y;

            fmt::print("{}, {}\n", pX, pY);


            /* startRay /= startRay.w;

            glm::vec4 ndc2   = glm::vec4(scaledX, scaledY, 1.0f, 1.0f);
            auto      endRay = renderer.toWorldCoords(ndc2);
            endRay /= endRay.w;


            //printVec(startRay, "Start ray Coordinates");
            //printVec(endRay, "End ray Coordinates");
            //fmt::print("{}, {}\n", result.x, result.y);
            //fmt::print("Scaled Coordinates: x: {}, y:{} \n", scaledX, scaledY);
            */
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.draw(graph.getGraph());

        glfwSwapBuffers(renderer.m_window);
        glfwPollEvents();
    }

    return 0;
}
