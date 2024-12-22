//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#include <glm/glm.hpp>
#include <string>
#include <utility>

namespace elements {
    class YObject {
    private:
        const std::string identifier;
        glm::mat4 modelMatrix{};

    public:
        YObject(std::string  identifier);
        virtual ~YObject() = default;

        [[nodiscard]] glm::mat4 getModelMatrix() const;

        virtual void draw();
        virtual void update();
    };
}
