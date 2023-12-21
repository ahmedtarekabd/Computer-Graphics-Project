#pragma once

#include <glad/gl.h>
#include "vertex.hpp"
#include <iostream>

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // TODO: (Req 2) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            // 1. Create Vertex Array Object
            glGenVertexArrays(1, &VAO);
            // Targeting VAO as current array
            glBindVertexArray(VAO);

            // 2. Create Vertex Buffer
            glGenBuffers(1, &VBO);
            // Targeting VBO -> Will be added to the VAO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Load data to VBO (currently binded buffer)
            glBufferData(GL_ARRAY_BUFFER,                  // Use targeted buffer (VBO)
                         vertices.size() * sizeof(Vertex), // Size of buffer
                         &vertices.front(),                // Data: Send the address of the first element, NOT THE OBJECT ADDRESS!
                         GL_STATIC_DRAW);                  // Copied to vram as soon as possible and left there,
                                                           // Source: https://computergraphics.stackexchange.com/questions/5712/gl-static-draw-vs-gl-dynamic-draw-vs-gl-stream-draw-does-it-matter

            // 3. Create Element Buffer
            glGenBuffers(1, &EBO);
            // Targeting EBO -> Will be added to the VAO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // Load data to VBO (currently binded buffer)
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         elements.size() * sizeof(unsigned int),
                         &elements.front(),
                         GL_STATIC_DRAW);
            // Save # of elements
            elementCount = static_cast<GLsizei>(elements.size());

            // 4. Config VAO: Attributes pointers
            // Position attribute
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            // Specify: Shader location, Number of position strides, Data type, Don't normalize, Vertex offset, Position offset from start
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

            // Color attribute
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            // Specify: Shader location, Number of color strides, Data type, Normalize, Vertex offset, Color offset from start
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

            // Texture Coordinates Attributes
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            // Specify: Shader location, Number of texture strides, Data type, Don't normalize, Vertex offset, Texture offset from start
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

            // Normal
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            // Specify: Shader location, Number of normal strides, Data type, Don't normalize, Vertex offset, Normal offset from start
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

            glBindVertexArray(0);
        }

        // this function should render the mesh
        void draw()
        {
            // TODO: (Req 2) Write this function
            //  Use the Vertex array
            glBindVertexArray(VAO);

            // Draw using indices provided in the EBO currently bound
            // Primitive type, Number of indicies, Data type of indicies, EBO offset (We use it so we pass 0 to start from the begining of the first buffer)
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);

            // Unbind VAO from Current vertex array state
            // Source: https://www.reddit.com/r/opengl/comments/f3sclv/why_some_tutorials_put_glbindvertexarray0_after/
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: (Req 2) Write this function

            // Source: https://docs.gl/gl3/glDeleteVertexArrays
            glDeleteVertexArrays(1, &VAO);
            // Source: https://docs.gl/gl3/glDeleteBuffers
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            VAO = 0;
            VBO = 0;
            EBO = 0;
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}