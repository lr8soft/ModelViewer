#include "RenderEvents.h"
#include "ModelManager.h"
#include "Utils/LogUtil.hpp"

void RenderEvents::OnInitModel(Event& event)
{
    void* data = const_cast<void*>(event.getEventData());
    if (data == nullptr)
    {
        LogUtil::printError("No model data!");
        return;
    }

    Model* model = static_cast<Model*>(data);
    if (model == nullptr)
    {
        LogUtil::printError("Not a model object!");
        return;
    }

    ModelManager::getInstance()->InitModel(model->getModelPath());
}
