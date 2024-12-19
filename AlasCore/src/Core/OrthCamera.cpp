#include "Core/OrthCamera.h"
#include "gtc/matrix_transform.hpp"

namespace Alas
{
    OrthCamera::OrthCamera(float left, float right, float top, float bottom)
    {
        glm::mat4 projection = glm::mat4(1.0f);
        _projectionMatrix = glm::ortho(left, right, top, bottom);

        _viewMatrix = glm::mat4(1.0f);
        _viewProjectionMatrix = _viewMatrix * _projectionMatrix;
    }

    void OrthCamera::CalculateViewMatrix()
    {
        _viewMatrix = glm::translate(glm::mat4(1.0f), -_position) * 
            glm::rotate(glm::mat4(1.0f), -_rotationZ, glm::vec3(0.0, 0.0, 1.0));

        _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
    }
} // namespace Alas
