#pragma once
#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_
#include <glm/glm.hpp>

namespace MathUtils 
{
    struct Transform
    {
        glm::vec3 position = glm::vec3(0.0);
        glm::vec3 motion = glm::vec3(0.0);
        glm::vec3 scale = glm::vec3(1.0);
        glm::vec3 rotation = glm::vec3(0.0);
    };

}

#endif