#pragma once
#ifndef _PUBLIC_RENDER_DATA_H_
#define _PUBLIC_RENDER_DATA_H_
#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "RenderData.h"
#include "Events/Event.h"

class PublicRenderData {
public:
    static std::multimap<std::string, Event&> renderingModelEvents;
    static std::vector<const char*> renderingModelNames;

    static std::mutex modelMutex;

    static const char ** getAllRenderingModels(int * modelCounts);

    static ShaderData shaderNoTexAndLight;
    static ShaderData shaderNoTex;
    static ShaderData shaderDepth;
    static ShaderData shaderDefault;
    static ShaderData shaderDefaultNoShadow;

    static unsigned int depthMapFrameBuffer, depthCubeMapFrameBuffer;
    static unsigned int depthMap, depthCubeMap;
};

#endif