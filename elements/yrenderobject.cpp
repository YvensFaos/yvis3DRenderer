//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "yrenderobject.h"
#include "../core/ymodel.h"
#include "../core/ymaterialinstance.h"
#include "../core/ymaterial.h"
#include "../core/yrenderer.h"

elements::YRenderObject::YRenderObject(const std::string &identifier, std::shared_ptr<core::YModel> model,
                                       std::shared_ptr<core::YMaterialInstance> materialInstance) : YObject(identifier),
    model(std::move(
        model)),
    materialInstance(
        std::move(
            materialInstance)) {
}

void elements::YRenderObject::draw(const core::YRenderer &renderer) {
    YObject::draw(renderer);
    materialInstance->drawModel(transform.getModelMatrix(), model,
                                renderer.getCamera()->getCachedViewProjectionMatrix());
}
