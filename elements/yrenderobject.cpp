//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "yrenderobject.h"
#include "../core/ymodel.h"
#include "../core/ymaterial.h"

elements::YRenderObject::YRenderObject(const std::string &identifier) : YObject(identifier) {}

void elements::YRenderObject::draw() {
    YObject::draw();
    material->drawModel(*model);
}
