#ifndef SHADER_H
#define SHADER_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

public:
    Shader(const std::string& filepath)
        : m_filepath(filepath)

    {
        m_shaderSource = readShader();
    }

    ~Shader()
    {
        glDeleteShader(m_shader);
    }


    [[nodiscard]] std::string readShader()
    {
        std::ifstream fileHandler;
        fileHandler.open(m_filepath);

        if (fileHandler.is_open()) {
            std::stringstream sstream;
            sstream << fileHandler.rdbuf();
            fileHandler.close();
            return sstream.str();
        } else {
            fmt::print("Unable to open file at filepath: {}", m_filepath);
        }
        return "";
    }

    [[nodiscard]] unsigned int compileShader(const bool isVertex)
    {
        const char* temp          = m_shaderSource.c_str();
        auto        compileShader = [&](auto id) {
            m_shader = id;
            glShaderSource(id, 1, &temp, nullptr);
            glCompileShader(id);
        };

        if (isVertex) {
            unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
            compileShader(vertexId);
            return vertexId;
        } else {
            unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
            compileShader(fragmentId);
            return fragmentId;
        }
    }

    [[nodiscard]] static unsigned int getUniform(const std::string& name, unsigned int program)
    {
        return glGetUniformLocation(program, name.c_str());
    }

    static void setUniformMat4(const std::string& name, unsigned int program, glm::mat4 matrix)
    {
        glUniformMatrix4fv(getUniform(name, program), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    static void setUniformMat4(unsigned int uniformLocation, glm::mat4 matrix)
    {
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }


private:
    const std::string m_filepath;
    std::string       m_shaderSource {};

    unsigned int m_shader;
};
#endif
