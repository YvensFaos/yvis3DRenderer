//
// Created by Yvens Serpa on 16/01/2025.
//

#include "ytexture.h"

#include <stb_image.h>
#include <string>
#include <GL/glew.h>

namespace core {
    YTexture::YTexture() : id(-1) {
    }

    YTexture::YTexture(const GLuint id, std::string path): id(id), path(std::move(path)) {
    }

    YTexture::YTexture(const YTexture &anotherTexture) {
        this->id = anotherTexture.id;
        this->path = anotherTexture.path;
        this->type = anotherTexture.type;
    }

    YTexture &YTexture::operator=(const YTexture &anotherTexture) {
        if (this != &anotherTexture) {
            this->id = anotherTexture.id;
            this->path = anotherTexture.path;
            this->type = anotherTexture.type;
        }
        return *this;
    }

    YTexture::~YTexture() {
        if(id != 1) {
            glDeleteTextures(1, &id);
        }
    }

    void YTexture::generateTextureId() {
        glGenTextures(1, &id);
    }

    YTexture::operator unsigned int() const {
        return id;
    }

    void YTexture::changeTextureParameter(const GLint textureParameter, const GLint textureParameterValue) const {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, textureParameter, textureParameterValue);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    YTexture YTexture::TextureFromFile(const char *path, const std::string &directory,
                                       const GLint minFilter, const GLint magFilter, const GLint textureWrapS,
                                       const GLint textureWrapT) {
        auto filename = std::string(path);
        if (!directory.empty()) {
            filename = directory + '/' + filename;
        }
        unsigned int textureID;
        glGenTextures(1, &textureID);
        printf("Loading texture [%d]: %s ... ", textureID, filename.c_str());

        int width, height, nrComponents;
        if (unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0)) {
            GLenum format = 0;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }

            printf("Loaded with %d x %d [Components: %d]!\r\n", width, height, nrComponents);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0,
                         static_cast<GLint>(format)
                         , width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        } else {
            printf("Texture failed to load at path: %s\n", path);
            stbi_image_free(data);
        }

        return {textureID, filename};
    }
}
