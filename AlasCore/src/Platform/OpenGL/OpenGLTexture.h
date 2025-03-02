#include "Renderer/Texture.h"

namespace Alas
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(const std::string& filepath);
        ~OpenGLTexture();

        
        virtual void Bind(uint32_t slot = 0) const override;
    };
} // namespace Alas
