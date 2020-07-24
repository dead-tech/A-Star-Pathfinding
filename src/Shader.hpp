#ifndef SHADER_H
#define SHADER_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#define __NODISCARD [[nodiscard]]

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
        if (m_filepath.empty()) {
            fmt::print("Invalid path to shader!\nError: {}", m_filepath);
            throw std::exception("Invalid path to shader!");
        }

        m_shaderSource = readShader();
    }

    __NODISCARD const std::string readShader() const
    {
        std::ifstream fileHandler;
        fileHandler.open(m_filepath);

        if (fileHandler.good()) {

            std::stringstream sstream;
            sstream << fileHandler.rdbuf();

            fileHandler.close();

            return sstream.str();
        } else {
            fmt::print("Unable to open file at filepath!\nError: {}", m_filepath);
            const std::string errorMessage { "Unable to open file at filepath:" + m_filepath };
            throw std::exception(errorMessage.c_str());
        }
    }

    __NODISCARD const unsigned int compileShader(const bool isVertex)
    {
        const char* temp = m_shaderSource.c_str();

        const auto compileShader = [&](const auto id) {
            m_shader = id;
            glShaderSource(id, 1, &temp, nullptr);
            glCompileShader(id);

            int success;
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);

            if (!success) {
                fmt::print("Failed to compile shader!\n");
                throw std::exception("Failed to compile shader!");
            }
        };

        if (isVertex) {
            const unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
            compileShader(vertexId);

            return vertexId;
        } else {
            const unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
            compileShader(fragmentId);

            return fragmentId;
        }
    }

    __NODISCARD static unsigned int getUniform(const std::string& name, unsigned int program)
    {
        return glGetUniformLocation(program, name.c_str());
    }

    static void setUniformBool(const std::string& name, unsigned int program, const bool boolean)
    {
        glUniform1i(getUniform(name, program), boolean);
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
