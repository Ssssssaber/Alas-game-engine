#include "Renderer/OrthCamera.h"
#include "gtc/matrix_transform.hpp"

namespace Alas
{
    OrthCamera::OrthCamera(uint32_t width, uint32_t height)
    {
        SetNewSize(width, height);
    }

    void OrthCamera::SetNewProjection(const glm::mat4& projectionMatrix)
    {
        _projectionMatrix = projectionMatrix;
        CalculateViewMatrix();
    }

    void OrthCamera::SetNewSize(uint32_t width, uint32_t height)
    {
        _height = height;
        _width = width;
        
        _projectionMatrix = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
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
