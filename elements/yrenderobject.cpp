//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "yrenderobject.h"

#include <utility>
#include "../core/ymodel.h"
#include "../core/ymaterialinstance.h"

elements::YRenderObject::YRenderObject(const std::string &identifier, std::shared_ptr<core::YModel> model, std::shared_ptr<core::YMaterialInstance> materialInstance) : YObject(identifier), model(std::move(model)), materialInstance(std::move(materialInstance)) {}

void elements::YRenderObject::draw() {
    YObject::draw();

//    material->drawModel(*model);
//    materialInstance.dra
}
