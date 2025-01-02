//
// Created by Yvens Serpa on 07/12/2024.
//

#include "ymodel.h"
#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

namespace core {
    YModel::YModel(const std::string &path) : transform(glm::mat4(1.0)) {
        loadModel(path);
    }

    YModel::YModel(const YModel &copyFrom) : transform(copyFrom.getModelMatrix()) {
        printf("YModel is being copied.\n");
        this->meshes = copyFrom.getMeshes();
        this->directory = copyFrom.getDirectory();
    }

    void YModel::loadModel(const std::string &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            return;
        }

        this->directory = path.substr(0, path.find_last_of('/'));
        printf("Loading object: %s\r\nDirectory: %s\r\n", path.c_str(), this->directory.c_str());
        glm::mat4 parentMat4 = YModel::aiMatrix4x4ToGlm(&scene->mRootNode->mTransformation);
        processNode(scene->mRootNode, scene, parentMat4);
    }

    void YModel::processNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentMat4) {
        glm::mat4 currentTransform = YModel::aiMatrix4x4ToGlm(&node->mTransformation);
        glm::mat4 resulting = parentMat4 * currentTransform;

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene, resulting));
        }
        for (unsigned int j = 0; j < node->mNumChildren; j++) {
            this->processNode(node->mChildren[j], scene, resulting);
        }
    }

    void YModel::draw(const GLuint programme, const GLenum mode, const bool renderWithTextures) const {
        for (const auto &mesh: meshes) {
            mesh.draw(programme, mode, renderWithTextures);
        }
    }

    void YModel::renderModels(GLuint modelMatrixUniform, GLuint programme, GLenum mode, bool renderWithTextures) const {
        const auto pointer = this->getMeshes();
        auto matrix = this->getModelMatrix();

        glUniformMatrix4fv(static_cast<GLint>(modelMatrixUniform), 1, GL_FALSE, glm::value_ptr(matrix));
        for (const auto &mesh: pointer) {
            mesh.draw(programme, mode, renderWithTextures);
        }
    }

    glm::mat4 YModel::aiMatrix4x4ToGlm(const aiMatrix4x4 *from) {
        glm::mat4 to;
        to[0][0] = static_cast<GLfloat>(from->a1);
        to[0][1] = static_cast<GLfloat>(from->b1);
        to[0][2] = static_cast<GLfloat>(from->c1);
        to[0][3] = static_cast<GLfloat>(from->d1);
        to[1][0] = static_cast<GLfloat>(from->a2);
        to[1][1] = static_cast<GLfloat>(from->b2);
        to[1][2] = static_cast<GLfloat>(from->c2);
        to[1][3] = static_cast<GLfloat>(from->d2);
        to[2][0] = static_cast<GLfloat>(from->a3);
        to[2][1] = static_cast<GLfloat>(from->b3);
        to[2][2] = static_cast<GLfloat>(from->c3);
        to[2][3] = static_cast<GLfloat>(from->d3);
        to[3][0] = static_cast<GLfloat>(from->a4);
        to[3][1] = static_cast<GLfloat>(from->b4);
        to[3][2] = static_cast<GLfloat>(from->c4);
        to[3][3] = static_cast<GLfloat>(from->d4);
        return to;
    }

    YMesh YModel::processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transformation) const {
        std::vector<YVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<YTexture> textures;

        glm::vec4 tv;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec4 vector4;
            YVertex vertex{};
            vector4.x = mesh->mVertices[i].x;
            vector4.y = mesh->mVertices[i].y;
            vector4.z = mesh->mVertices[i].z;
            vector4.w = 1.0f;

            tv = transformation * vector4;
            vertex.Position.x = tv.x;
            vertex.Position.y = tv.y;
            vertex.Position.z = tv.z;

            vector4.x = mesh->mNormals[i].x;
            vector4.y = mesh->mNormals[i].y;
            vector4.z = mesh->mNormals[i].z;
            vertex.Normal.x = vector4.x;
            vertex.Normal.y = vector4.y;
            vertex.Normal.z = vector4.z;

            if (mesh->mTangents) {
                vector4.x = mesh->mTangents[i].x;
                vector4.y = mesh->mTangents[i].y;
                vector4.z = mesh->mTangents[i].z;
            }

            vertex.Tangent.x = vector4.x;
            vertex.Tangent.y = vector4.y;
            vertex.Tangent.z = vector4.z;

            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        aiFace face;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<YTexture> diffuseMaps =
                    loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<YTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                      "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            std::vector<YTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }
        return {vertices, indices, textures};
    }

    std::vector<YTexture> YModel::loadMaterialTextures(const aiMaterial *mat, const aiTextureType type,
                                                       const std::string &typeName) const {
        std::vector<YTexture> textures;
        for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            YTexture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = std::string(str.C_Str());
            textures.push_back(texture);
        }
        return textures;
    }

    unsigned int YModel::TextureFromFile(const char *path, const std::string &directory, bool gamma,
                                         const GLint minFilter, const GLint magFilter, const GLint textureWrapS,
                                         const GLint textureWrapT) {
        std::string filename = std::string(path);
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

        return textureID;
    }

    const std::vector<YMesh> &YModel::getMeshes() const {
        return this->meshes;
    }

    std::string YModel::getDirectory() const {
        return this->directory;
    }

    void YModel::translate(const glm::vec3 translateTo) {
        this->transform.translate(translateTo);
    }

    void YModel::rotate(const glm::vec3 rotateTo) {
        this->transform.rotate(rotateTo);
    }

    void YModel::scale(glm::vec3 scaleTo) {
        this->transform.scale(scaleTo);
    }

    glm::mat4 YModel::getModelMatrix() const {
        return this->transform.getModelMatrix();
    }

    glm::vec4 YModel::getPosition() const {
        return this->transform.getPosition();
    }

    void YModel::setPosition(glm::vec3 position) {
        this->transform.setPosition(position);
    }

    math::YBoundingBox YModel::getBoundingBox() const {
        glm::vec3 min(+999999.0f, +999999.0f, +999999.0f);
        glm::vec3 max(-999999.0f, -999999.0f, -999999.0f);

        for (const auto &meshe: this->meshes) {
            math::YBoundingBox abb = meshe.getBoundingBox();
            min.x = std::min(min.x, abb.getMin().x);
            min.y = std::min(min.y, abb.getMin().y);
            min.z = std::min(min.z, abb.getMin().z);
            max.x = std::max(max.x, abb.getMax().x);
            max.y = std::max(max.y, abb.getMax().y);
            max.z = std::max(max.z, abb.getMax().z);
        }

        return {min, max};
    }

    void YModel::renderModelsInList(const std::vector<YModel> &list, const GLuint modelMatrixUniform,
                                    const GLuint programme, const GLenum mode,
                                    const bool renderWithTextures) {
        for (auto &YModelIterator: list) {
            const std::vector<YMesh> &pointer = YModelIterator.getMeshes();
            auto modelMatrix = YModelIterator.getModelMatrix();
            for (const auto &i: pointer) {
                glUniformMatrix4fv(
                    static_cast<GLint>(modelMatrixUniform)
                    , 1, GL_FALSE, glm::value_ptr(modelMatrix));
                i.draw(programme, mode, renderWithTextures);
            }
        }
    }

    void YModel::renderPatchesModelsInList(const std::vector<YModel> &list, const GLuint modelMatrixUniform,
                                           const GLuint programme,
                                           const bool renderWithTextures) {
        for (auto &YModelIterator: list) {
            const std::vector<YMesh> &pointer = YModelIterator.getMeshes();
            auto modelMatrix = YModelIterator.getModelMatrix();
            for (const auto &i: pointer) {
                glUniformMatrix4fv(
                    static_cast<GLint>(modelMatrixUniform)
                    , 1, GL_FALSE, glm::value_ptr(modelMatrix));
                i.drawPatches(programme, renderWithTextures);
            }
        }
    }

    const YMesh &YModel::getMeshAt(int index) const {
        return this->meshes[index];
    }

    YModel &YModel::operator=(const YModel &copyFrom) {
        if (this != &copyFrom) {
            this->transform.setModelMatrix(copyFrom.getModelMatrix());
            this->meshes = copyFrom.getMeshes();
            this->directory = copyFrom.getDirectory();
        }
        return *this;
    }
} // core
