#include "Renderer/Texture.h"

namespace Alas
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(UID uid, uint32_t width, uint32_t height);
        OpenGLTexture(UID uid, const std::string& filepath);
        ~OpenGLTexture();

        
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void SetData(void* data, uint32_t size) const override;
    };
} // namespace Alas
