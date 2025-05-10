#pragma once 
#include "glm.hpp"
namespace Alas {

    struct ShaderSourceCode {
        std::string VertexShaderSource;
        std::string FragmentShaderSource;

        ShaderSourceCode(std::string vertexShaderSource, std::string fragmentShaderSource) :
            VertexShaderSource(vertexShaderSource), FragmentShaderSource(fragmentShaderSource)
        {}
    }; 

    class Shader 
    {
    public:
        static Shared<Shader> Create(const std::string& filepath, UID uid = NULL_UID);
        static Shared<Shader> Create(UID uid, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        
        Shader(UID uid) : _resourceID(uid) { }
        UID GetUID() { return _resourceID; }

        virtual ~Shader() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // utility uniform functions
        virtual void setBool(const std::string &name, bool value) const = 0;
        virtual void setInt(const std::string &name, int value) const = 0;
        virtual void setFloat(const std::string &name, float value) const = 0;
        virtual void setVec2(const std::string &name, const glm::vec2 &value) const = 0;
        virtual void setVec2(const std::string &name, float x, float y) const = 0;
        virtual void setVec3(const std::string &name, const glm::vec3 &value) const = 0;
        virtual void setVec3(const std::string &name, float x, float y, float z) const = 0;
        virtual void setVec4(const std::string &name, const glm::vec4 &value) const = 0;
        virtual void setVec4(const std::string &name, float x, float y, float z, float w) const = 0;
        virtual void setMat2(const std::string &name, const glm::mat2 &mat) const = 0;
        virtual void setMat3(const std::string &name, const glm::mat3 &mat) const = 0;
        virtual void setMat4(const std::string &name, const glm::mat4 &mat) const = 0;
        
    private:
        static ShaderSourceCode* ParseShaderFile(const std::string& filepath);
        
    private:
        UID _resourceID;
        uint32_t _rendererId;
    };
}