#include "PublicRenderData.h"

std::multimap<std::string, Event&> PublicRenderData::renderingModelEvents;
std::vector<const char*> PublicRenderData::renderingModelNames;

std::mutex PublicRenderData::modelMutex;

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
