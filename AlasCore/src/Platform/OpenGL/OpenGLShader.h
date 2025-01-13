#include "Renderer/Shader.h"

namespace Alas
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(std::string& vertexShaderSource, std::string& fragmentShaderSource);
        ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        // utility uniform functions
        virtual void setBool(const std::string &name, bool value) const override;
        virtual void setInt(const std::string &name, int value) const override;
        virtual void setFloat(const std::string &name, float value) const override;
        virtual void setVec2(const std::string &name, const glm::vec2 &value) const override;
        virtual void setVec2(const std::string &name, float x, float y) const override;
        virtual void setVec3(const std::string &name, const glm::vec3 &value) const override;
        virtual void setVec3(const std::string &name, float x, float y, float z) const override;
        virtual void setVec4(const std::string &name, const glm::vec4 &value) const override;
        virtual void setVec4(const std::string &name, float x, float y, float z, float w) const override;
        virtual void setMat2(const std::string &name, const glm::mat2 &mat) const override;
        virtual void setMat3(const std::string &name, const glm::mat3 &mat) const override;
        virtual void setMat4(const std::string &name, const glm::mat4 &mat) const override;
        
    private:
        uint32_t _rendererId;
    };
} // namespace Alas
