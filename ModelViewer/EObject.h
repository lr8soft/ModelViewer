#pragma once
#ifndef _ENGINE_OBJECT_
#define _ENGINE_OBJECT_

#include "Utils/Timer.h"
#include "Utils/MathUtils.h"

#include <string>

class EObject
{
private:
    MathUtils::Transform transform;

    std::string objectId;
    Timer objectTimer;


public:
    void OnUpdate();
    void OnRender();

};

#endif