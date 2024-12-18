#pragma once
#include "glm.hpp"

namespace Alas
{
    class OrthCamera
    {
    public:
        OrthCamera(float left, float right, float top, float bottom);

        const glm::vec3& GetPosition() { return _position; }
        void SetPosition(const glm::vec3& position) { _position = position;  CalculateViewMatrix(); }
        
        const float GetRotation() { return _rotationZ; }
        void SetRotation(float rotation) { _rotationZ = rotation;  CalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() { return _projectionMatrix; }
        const glm::mat4& GetViewMatrix() { return _viewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() { return _viewProjectionMatrix; }

    private:
        void CalculateViewMatrix();

    private:
        float _width, _height;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
        glm::mat4 _viewProjectionMatrix;

        glm::vec3 _position;
        float _rotationZ;
    };
} // namespace Alas
