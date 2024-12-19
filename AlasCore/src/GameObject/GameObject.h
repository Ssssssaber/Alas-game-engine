#pragma once

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace Alas
{
    class GameObject
    {
    public:
        GameObject(VertexArray* vertexArray, Shader* shader);

        const glm::vec3& GetPosition() { return _position; }
        void SetPosition(const glm::vec3& position) { _position = position;  CalculateModelMatrix(); }
        
        const glm::vec3 GetRotation() { return _rotation; }
        void SetRotation(glm::vec3 rotation) { _rotation = rotation;  CalculateModelMatrix(); }

        const glm::vec3 GetScale() {return _scale; }
        void SetScale(glm::vec3 scale) { _scale = scale;  CalculateModelMatrix(); }

        const glm::mat4& GetModelMatrix() { return _modelMatrix; }
        
        const std::shared_ptr<Shader> GetShader() { return _shader; }
        const std::shared_ptr<VertexArray> GetVertexArray() { return _vertexArray; }

    private:
        void CalculateModelMatrix();

    private:
        std::shared_ptr<Shader> _shader;
        std::shared_ptr<VertexArray> _vertexArray;

        glm::vec3 _position = glm::vec3(0.0f);
        glm::vec3 _rotation = glm::vec3(0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);

        glm::mat4 _modelMatrix;
        
    };
} // namespace Alas
