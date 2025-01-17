//
// Created by Yvens Serpa on 16/01/2025.
//

#pragma once
#include <string>
#include <GL/glew.h>

namespace core {
    struct YTexture {
        GLuint id;
        std::string type;
        std::string path;

        explicit YTexture();

        YTexture(GLuint id, std::string path);

        YTexture(const YTexture &);

        YTexture &operator=(const YTexture &anotherTexture);

        ~YTexture();

        void generateTextureId();

        explicit operator GLuint() const;

        void changeTextureParameter(GLint textureParameter, GLint textureParameterValue) const;

        static YTexture TextureFromFile(const char *path, const std::string &directory,
                                        GLint minFilter, GLint magFilter, GLint textureWrapS,
                                        GLint textureWrapT);
    };
}
