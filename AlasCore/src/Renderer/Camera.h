#pragma once

#include "glm.hpp"

namespace AGS
{
    class Camera
    {
    public:
        Camera();

        glm::mat4 ComputeModelMatrix();
    private:
        
    };
} // namespace AGS
