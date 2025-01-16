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
        GameObject(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader, std::string name = "GameObject");

        virtual void Update() {}
        void InnerUpdate() { SetColor(_color);  CalculateModelMatrix(); }

        glm::vec3& GetColor() { return _color; }
        void SetColor(const glm::vec3 color) { _color = color; ImplementSetColor(_color); }

        int GetId() { return _id; }

        std::string* GetName() { return &_name; }
        void SetName(std::string name) { _name = name; }

        glm::vec3& GetPosition() { return _position; }
        void SetPosition(const glm::vec3& position) { _position = position;  CalculateModelMatrix(); }
        
        glm::vec3& GetRotation() { return _rotation; }
        void SetRotation(glm::vec3 rotation) { _rotation = rotation;  CalculateModelMatrix(); }

        glm::vec3& GetScale() {return _scale; }
        void SetScale(glm::vec3 scale) { _scale = scale;  CalculateModelMatrix(); }

        const glm::mat4& GetModelMatrix() { return _modelMatrix; }
        
        const Shared<Shader> GetShader() { return _shader; }
        const Shared<VertexArray> GetVertexArray() { return _vertexArray; }

    private:
        void ImplementSetColor(const glm::vec3& color) 
        {
            _shader->setVec4("u_Color", color.x, color.y, color.z, 1.0f);
        }
        void CalculateModelMatrix();

    private:
        static uint64_t _ID;

        int _id;
        std::string _name;
        Shared<Shader> _shader;
        Shared<VertexArray> _vertexArray;

        glm::vec3 _color = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 _position = glm::vec3(0.0f);
        glm::vec3 _rotation = glm::vec3(0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);

        glm::mat4 _modelMatrix;
        
    };
} // namespace Alas
