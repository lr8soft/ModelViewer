#pragma once
#ifndef _RENDER_DATA_H_
#define _RENDER_DATA_H_

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderData {
    std::string modelName;
    unsigned int shaderId;
    unsigned int textureStartIndex = 0;
};

struct ShaderData {
    std::string shaderName;
    std::string vertexShader;
    std::string fragShader;
};

union UniformValue {
    glm::mat4 matrix5;
    glm::vec4 vector4;
    glm::vec3 vector3;
    glm::vec2 vector2;

    float value1;
    int num0;
};

struct UniformData {
    std::string attrName;
    UniformValue value;
    unsigned int valueIndex = 0;
    unsigned int shaderId;
};

#endif