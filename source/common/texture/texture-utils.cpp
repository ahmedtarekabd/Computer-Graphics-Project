#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D *our::texture_utils::empty(GLenum format, glm::ivec2 size)
{
    our::Texture2D *texture = new our::Texture2D();
    // TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    texture->bind();
    // glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, NULL);
    glTexStorage2D(GL_TEXTURE_2D, 1, format, size.x, size.y);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // texture->unbind();

    return texture;
}

our::Texture2D *our::texture_utils::loadImage(const std::string &filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
    // Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    // We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    // Load image data and retrieve width, height and number of channels in the image
    // The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    // Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char *pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if (pixels == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D *texture = new our::Texture2D();
    // Bind the texture such that we upload the image data to its storage
    texture->bind();
    // TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"

    /*
    sets the pixel storage modes for pixel transfer operations
    telling OpenGL that each row of pixel data in memory is tightly packed with no additional padding
    */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /**  to be editeddd  0: Keep number of channels the same as in the image file  */

    // GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data
    /*
       GL_RG:  suitable for grayscale and alpha data
       GL_RED: suitable for grayscale data.
       GL_RGB: suitable for RGB
    */
    //    std::cout << "channel: " << channels << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    // switch(channels){
    //     case 1:
    //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    //        break;
    //     case 2:
    //       glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, size.x, size.y, 0, GL_RG, GL_UNSIGNED_BYTE, pixels);
    //        break;
    //     case 3:

    //     //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    //       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    //        break;
    //     case 4:
    //       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    //        break;
    // }

    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D); // is used to generate mipmaps for a texture
    texture->unbind();

    stbi_image_free(pixels); // Free image data after uploading to GPU
    return texture;
}

//
our::Texture2D *our::texture_utils::textureColor(const glm::vec4 &color)
{

    our::Texture2D *texture = new our::Texture2D();
    texture->bind();

    // Sets the pixel storage modes for pixel transfer operations
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_FLOAT, &color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Repeat the same color for all pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D); // is used to generate mipmaps for a texture

    texture->unbind();
    return texture;
}