#pragma once
#ifndef _RENDER_DATA_H_
#define _RENDER_DATA_H_

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FrameInfo {
public:
    static float FrameBottom, FrameTop, FrameRight, FrameLeft;
    static int ScreenWidth, ScreenHeight;
};

struct Transform {
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0);
    glm::vec3 rotation = glm::vec3(0.0);
};

struct RenderData {
    Transform transform;
    std::string modelName;
    bool renderShadow = false;
};

struct ShaderData {
    std::string shaderName;
    std::string vertexShader;
    std::string fragShader;
    unsigned int shaderId = 0;
};

struct UniformValue {
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
};

#endif