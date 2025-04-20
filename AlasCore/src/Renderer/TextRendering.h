#pragma once
#include "glm.hpp"

namespace Alas
{
    class TextRendering
    {        
    public:
        TextRendering() = default;
        static Shared<TextRendering> Create();

        virtual void Init() = 0;
        virtual void RenderText(const std::string& text, glm::vec3 position, float rotation, float scale, glm::vec4 color) = 0;
    };
} // namespace Alas