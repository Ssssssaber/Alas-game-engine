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
        virtual void RenderText(const std::string& text, glm::vec3 position, const glm::vec2& scale) = 0;
    };
} // namespace Alas