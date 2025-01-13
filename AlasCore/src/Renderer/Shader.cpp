#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace Alas {
    Shared<Shader> Shader::Create(std::string& vertexShaderSource, std::string& fragmentShaderSource)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    ALAS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Shared<Shader>(new OpenGLShader(vertexShaderSource, fragmentShaderSource));
        }

        ALAS_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}