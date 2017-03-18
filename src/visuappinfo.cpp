#include "visuappinfo.h"

VisuAppInfo* VisuAppInfo::instance = nullptr;

VisuAppInfo* VisuAppInfo::getInstance()
{
    if (instance == nullptr)
    {
        instance = new VisuAppInfo();
        instance->inEditorMode = false;
        instance->configWrong = false;
        instance->server = nullptr;
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

void VisuAppInfo::setConfigWrong(const QString& issue)
{
    getInstance()->configWrong = true;
    getInstance()->configIssues.append(issue);
}

const QStringList& VisuAppInfo::getConfigIssues()
{
    return getInstance()->configIssues;
}

int VisuAppInfo::argsSize()
{
    return getInstance()->cliArgs.size();
}

const QString& VisuAppInfo::getCLIArg(CLI_Args arg)
{
    return getInstance()->cliArgs[(int)arg];
}

void VisuAppInfo::setCLIArgs(int argc, char* argv[])
{
    QStringList& args = getInstance()->cliArgs;
    for (int i=0 ; i<argc ; ++i)
    {
        args.append(QString(argv[i]));
    }
}

void VisuAppInfo::setServer(VisuServer *srv)
{
    getInstance()->server = srv;
}

VisuServer* VisuAppInfo::getServer()
{
    return getInstance()->server;
}
