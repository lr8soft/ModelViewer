#include "ModelManager.h"

ModelManager* ModelManager::pInstance = nullptr;
ModelManager * ModelManager::getInstance()
{
    if (pInstance == nullptr)
    {
        pInstance = new ModelManager;
    }
    return pInstance;
}

void ModelManager::LoadModel(const std::string & modelFileName)
{
    Model* model = GetModel(modelFileName);
    if (model == nullptr)
    {
        model = new Model(modelFileName);
        globalModelGroup[modelFileName] = model;
    }
}


Model * ModelManager::GetModel(const std::string & modelFileName)
{
    if (globalModelGroup.find(modelFileName) != globalModelGroup.end())
    {
        return globalModelGroup[modelFileName];
    }
    return nullptr;
}

void ModelManager::InitModel(const std::string & modelFileName)
{
    Model* model = GetModel(modelFileName);
    if (model && !model->isModelInit())
    {
        model->onModelInit();
    }
}

void ModelManager::RenderModel(const std::string & modelFileName, unsigned int shader, bool useInsideTexture, unsigned int textureStartIndex)
{
    Model* model = GetModel(modelFileName);
    if (model && model->isModelInit())
    {
        model->onModelRender(shader, useInsideTexture, textureStartIndex);
    }
}