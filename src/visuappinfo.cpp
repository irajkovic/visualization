#include "visuappinfo.h"

VisuAppInfo* VisuAppInfo::instance = nullptr;

VisuAppInfo* VisuAppInfo::getInstance()
{
    if (instance == nullptr)
    {
        instance = new VisuAppInfo();
        instance->inEditorMode = false;
        instance->configWrong = false;
    }
    return instance;
}

bool VisuAppInfo::isInEditorMode()
{
    return getInstance()->inEditorMode;
}

void VisuAppInfo::setInEditorMode(bool mode)
{
    getInstance()->inEditorMode = mode;
}

bool VisuAppInfo::isConfigWrong()
{
    return getInstance()->configWrong;
}

void VisuAppInfo::setConfigWrong(bool wrong)
{
    getInstance()->configWrong = wrong;
}
