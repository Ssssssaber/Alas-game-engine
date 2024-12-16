#include "Camera.h"

namespace AGS
{
    Camera::Camera()
    {
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
        
    }

    glm::mat4 Camera::ComputeModelMatrix()
    {
        float l, r, b, t; // left right bottom top
        float f, n; 
    }
} // namespace AGS
