#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        // Tarek
        ShaderProgram()
        {
            // TODO: (Req 1) Create A shader program
            program = glCreateProgram();
        }
        // Tarek
        ~ShaderProgram()
        {
            // TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        // Tarekkkkkkkkkkkkkkkkkkkkkkkk
        bool attach(const std::string &filename, GLenum type) const;

        // Tarek
        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        // Zaki
        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
        }

        // Osama
        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }

        // Osama
        void set(const std::string &uniform, GLuint value)
        {
            // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }

        // Osama
        void set(const std::string &uniform, GLint value)
        {
            // TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }

        // Waleed
        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        // Waleed
        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: (Req 1) Send the given 3D vector value to the given uniform
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        // Waleed
        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        // Zaki
        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        // TODO: (Req 1) Delete the copy constructor and assignment operator.
        // Question: Why do we delete the copy constructor and assignment operator?
    };

}

#endif