//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "yloadedscene.h"
#include "../utils/yluahelper.h"
#include "../core/ymaterialinstance.h"

namespace scenes {
    YLoadedScene::YLoadedScene(core::YRenderer &renderer, const std::string &file, const int width,
                               const int height) : YScene(renderer, file, width, height) {

        auto clearColor = utils::YLuaHelper::readVec4FromTable("clearColor", luaHandler);
        renderer.changeClearColor(clearColor);

        luaHandler.loadTable("scene");
        //Read and generate materials
        if(luaHandler.getTableFromTable("materials")) {
            auto numberOfMaterials = luaHandler.getLength();
            printf("Found %d materials.\n", numberOfMaterials);

            for (int i = 1; i <= numberOfMaterials; ++i) {
                if(luaHandler.getTableFromTable(i)) {
                    auto materialName = luaHandler.getStringFromTable("name");
                    auto vertexShaderName = luaHandler.getStringFromTable("vertexShader");
                    auto vertexShader = luaHandler.getGlobalString(vertexShaderName);
                    auto fragmentShaderName = luaHandler.getStringFromTable("fragmentShader");
                    auto fragmentShader = luaHandler.getGlobalString(fragmentShaderName);
                    printf("%s\nVertex Shader[%s]: %s\n\nFragment Shader[%s]: %s\n\n", materialName.c_str(), vertexShaderName.c_str(), vertexShader.c_str(), fragmentShaderName.c_str(), fragmentShader.c_str());

                    if(!materials.contains(materialName)) {
                        materials.emplace(materialName,std::make_shared<core::YMaterial>(materialName, vertexShader, fragmentShader));
                    }
                    luaHandler.popTable();
                }
            }
            luaHandler.popTable();
        }

        //Read and generate models
        if(luaHandler.getTableFromTable("models")) {
            auto numberOfModels = luaHandler.getLength();
            printf("Found %d models.\n", numberOfModels);

            for (int i = 1; i <= numberOfModels; ++i) {
                if(luaHandler.getTableFromTable(i)) {
                    auto modelName = luaHandler.getStringFromTable("name");
                    auto model = luaHandler.getStringFromTable("model");
                    printf("Loading model named %s.\n", model.c_str());

                    luaHandler.loadTable(model);
                    auto modelFile = luaHandler.getStringFromTable("file");
                    luaHandler.popTable();

                    if (!models.contains(modelFile)) {
                        models.emplace(modelFile, std::make_shared<core::YModel>(modelFile));
                    }

                    auto yModel = models[modelFile];

                    auto materialName = luaHandler.getStringFromTable("material");
                    auto yMaterial = materials[materialName];

                    printf("Generating YVRenderObject %s...\n", modelName.c_str());

                    auto materialInstance = std::make_shared<core::YMaterialInstance>(yMaterial, "uniforms", luaHandler);
                    auto renderObject = std::make_shared<elements::YRenderObject>(modelName, yModel, materialInstance);

                    auto modelPosition = utils::YLuaHelper::readVec3FromTableInTable("pos", luaHandler);
                    auto modelScale = utils::YLuaHelper::readVec3FromTableInTable("sca", luaHandler);
                    auto modelRotation = utils::YLuaHelper::readVec3FromTableInTable("rot", luaHandler);

                    renderObject->transform.setPosition(modelPosition);
                    renderObject->transform.scale(modelScale);
                    renderObject->transform.rotate(modelRotation);

                    printf("Complete!\n");

                    objects.push_back(renderObject);
                    luaHandler.popTable();
                }
            }
        }
        luaHandler.popTable();

        core::YCamera &camera = renderer.getCamera();
        printf("Loading camera information...");
        utils::YLuaHelper::setupCameraPosition("camera", camera, luaHandler);
        printf(" complete!\n");
    }

    void YLoadedScene::renderImpl() {
        renderer.getCamera().cacheViewProjectionMatrix(static_cast<float>(width), static_cast<float>(height));
        for (const auto& object: objects) {
            object->draw(renderer);
            object->update();
        }
    }
} // scenes