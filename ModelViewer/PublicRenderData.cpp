#include "PublicRenderData.h"

std::multimap<std::string, Event&> PublicRenderData::renderingModelEvents;
std::vector<const char*> PublicRenderData::renderingModelNames;

std::mutex PublicRenderData::modelMutex;

ShaderData PublicRenderData::shaderNoTexAndLight{ "No_Tex_And_Light", "Assets/no_light.vert", "Assets/no_texture_and_light.frag" };
ShaderData PublicRenderData::shaderNoTex{ "No_Light", "Assets/no_light.vert", "Assets/no_light.frag" };
ShaderData PublicRenderData::shaderDepth{ "Depth", "Assets/depth.vert", "Assets/depth.frag" };
ShaderData PublicRenderData::shaderDefault{ "Default", "Assets/default.vert", "Assets/default.frag" };
ShaderData PublicRenderData::shaderDefaultNoShadow{ "DefaultNoShadow", "Assets/no_shadow.vert", "Assets/no_shadow.frag" };

unsigned int PublicRenderData::depthMapFrameBuffer = 0, PublicRenderData::depthCubeMapFrameBuffer = 0;
unsigned int PublicRenderData::depthMap = 0, PublicRenderData::depthCubeMap = 0;

const char ** PublicRenderData::getAllRenderingModels(int * modelCounts)
{
    int count = renderingModelNames.size();
    (*modelCounts) = count;
    if (count == 0)
    {
        static const char* nullStr = "";
        return &nullStr;
    }

    return &PublicRenderData::renderingModelNames[0];
}
