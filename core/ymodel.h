//
// Created by Yvens Serpa on 07/12/2024.
//

#pragma once

#include <string>
#include "ymesh.h"
#include "ytransform.h"
#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace core {
    class YModel {
        core::YTransform transform;
        std::vector<YMesh> meshes;
        std::string directory;

    public:
        explicit YModel(const std::string &path);

        YModel(const YModel &copyFrom);

        void draw(GLuint programme, GLenum mode = GL_TRIANGLES, bool renderWithTextures = true) const;

        void renderModels(GLuint modelMatrixUniform, GLuint programme, GLenum mode = GL_TRIANGLES,
                          bool renderWithTextures = true) const;

        [[nodiscard]] const std::vector<YMesh> &getMeshes() const;

        [[nodiscard]] const YMesh &getMeshAt(int index) const;

        void translate(glm::vec3 translateTo);

        void rotate(glm::vec3 rotateTo);

        void scale(glm::vec3 scaleTo);

        [[nodiscard]] glm::mat4 getModelMatrix() const;

        [[nodiscard]] glm::vec4 getPosition() const;

        [[nodiscard]] std::string getDirectory() const;

        void setPosition(glm::vec3 position);

        [[nodiscard]] math::YBoundingBox getBoundingBox() const;

        YModel &operator=(const YModel &copyFrom);

    private:
        void loadModel(const std::string &path);

        void processNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentMat4);

        YMesh processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transformation) const;

        std::vector<YTexture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string &typeName) const;

    public:
        static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false,
                                            GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR,
                                            GLint textureWrapS = GL_CLAMP_TO_BORDER,
                                            GLint textureWrapT = GL_CLAMP_TO_BORDER);

        static void renderModelsInList(const std::vector<YModel> &list, GLuint modelMatrixUniform, GLuint programme,
                                       GLenum mode = GL_TRIANGLES, bool renderWithTextures = true);

        static void renderPatchesModelsInList(const std::vector<YModel> &list, GLuint modelMatrixUniform, GLuint programme,
                                              bool renderWithTextures = true);

        static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 *from);
    };
} // core
