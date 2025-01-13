#include "GameObject.h"

namespace Alas
{
    uint64_t GameObject::_ID = 0;

    GameObject::GameObject(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader, std::string name)
        : _name(name)
    {
        _id = _ID;
        _ID++;

        _vertexArray = vertexArray;
        _shader = shader;

        _shader->Bind();
        SetColor(_color);
        
        _modelMatrix = glm::mat4(1.0f);
    }

    void GameObject::CalculateModelMatrix()
    {
        _modelMatrix = glm::translate(glm::mat4(1.0f), _position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), _scale);
    }
} // namespace Alas
