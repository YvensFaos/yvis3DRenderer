//
// Created by Yvens Serpa on 01/01/2025.
//

#pragma once
#include <memory>

namespace elements {
    class YObject;
}

namespace view {

class YObjectUI {
    std::shared_ptr<elements::YObject> selfObject;

public:
    explicit YObjectUI(std::shared_ptr<elements::YObject> object);
    ~YObjectUI() = default;
    YObjectUI(const YObjectUI&) = default;

    void render() const;

    std::string getIdentifier() const;
};

} // view

