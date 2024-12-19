#include "GameObject.h"

namespace Alas
{
    GameObject::GameObject(VertexArray* vertexArray, Shader* shader)
    {
        _vertexArray.reset(vertexArray);
        _shader.reset(shader);

        _shader->Bind();
        
        _modelMatrix = glm::mat4(1.0f);
    }

    void GameObject::CalculateModelMatrix()
    {
        _modelMatrix = glm::translate(glm::mat4(1.0f), _position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), _scale);
    }
} // namespace Alas
