#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <sstream>
#include <fstream>

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

    Shared<Shader> Shader::Create(const std::string& filepath)
    {
        ShaderSourceCode* source = ParseShaderFile(filepath);
        return Create(source->vertexShaderSource, source->fragmentShaderSource);
    }

    ShaderSourceCode* Shader::ParseShaderFile(const std::string& filepath)
    {
        std::fstream stream(filepath);
        
        enum ShaderType {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1
        } shaderType;

        std::stringstream ss[2];
        std::string line;
        while (getline(stream, line))
        {
            if (line.find("shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    shaderType = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    shaderType = ShaderType::FRAGMENT;    
                }
            } 
            else
            {
                ss[shaderType] << line << "\n";
            }
        }

        return new ShaderSourceCode{
            .vertexShaderSource = ss[0].str(),
            .fragmentShaderSource = ss[1].str()
        };
    }
}