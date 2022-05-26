#pragma once
#ifndef _ENGINE_MANAGER_H_
#define _ENGINE_MANAGER_H_

#include <map>

#include "Utils/Timer.h"

class EngineManager 
{
private:
    static EngineManager* pInstance;

    //std::map<std::string, >

    EngineManager();
public:
    static EngineManager* getInstance();

    void onLogicalWork();
};

#endif