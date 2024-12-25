//
// Created by Yvens Serpa on 09/12/2024.
//

#include "yskybox.h"

#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "../core/yshader.h"

namespace elements {
    float vertices[] = {
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    std::string YSkybox::defaultVertexShader =
            "#version 400\n"
            "   layout (location = 0) in vec3 aPos;\n"
            "   uniform mat4 vpMatrix;\n"
            "   out vec3 texCoord;\n"
            "   void main()\n"
            "   {\n"
            "       texCoord = aPos;\n"
            "       vec4 pos = vpMatrix * vec4(aPos, 1.0);\n"
            "       gl_Position = pos.xyww;\n"
            "   }\n";

    std::string YSkybox::defaultFragmentShader =
            "#version 400\n"
            "   in vec3 texCoord;\n"
            "   uniform samplerCube skybox;\n"
            "   out vec4 frag_colour;\n"
            "   void main()\n"
            "   {   \n"
            "       vec4 color = texture(skybox, texCoord);\n"
            "       frag_colour = color;\n"
            "   }\n";

    YSkybox::YSkybox(const std::vector<std::string> &faces) : cubeMapTextureID(0), width(0),
                                                              height(0), channels(0), VAO(0), VBO(0), skyboxUniform(0),
                                                              faces(faces) {
        vertexShader = core::YShader::generateShader(defaultVertexShader, GL_VERTEX_SHADER);
        fragmentShader = core::YShader::generateShader(defaultFragmentShader, GL_FRAGMENT_SHADER);
        programme = core::YShader::generateProgram(vertexShader, fragmentShader);
        skyboxUniform = glGetUniformLocation(programme, "skybox");
        viewProjectionUniform = glGetUniformLocation(programme, "vpMatrix");

        glGenTextures(1, &cubeMapTextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

        for (GLuint i = 0; i < faces.size(); i++) {
            printf("Load Face %d: %s\n", i + 1, faces[i].c_str());
            const unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    }

    void YSkybox::render(glm::mat4 viewProjectionMatrix) const {
        glUseProgram(programme);
        glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));

        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        glUniform1i(skyboxUniform, 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }
} // elements
