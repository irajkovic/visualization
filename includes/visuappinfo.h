#ifndef VISUAPPINFO_H
#define VISUAPPINFO_H

#include <QString>
#include <QStringList>

class VisuServer;

class VisuAppInfo
{

public:
    enum class EditorArgs
    {
        PROGRAM_NAME,
        EDITOR_FLAG
    };

    enum class RunArgs
    {
        PROGRAM_NAME,
        CONFIG_PATH,
        SERIAL_PORT_NAME,
        BAUD_RATE
    };

    static bool isInEditorMode();
    static void setInEditorMode(bool mode);

    static bool isConfigWrong();
    static void setConfigWrong(bool wrong);
    static void setConfigWrong(bool wrong, const QString& issue);
    static const QStringList& getConfigIssues();
    static const QString& getCLIArg(RunArgs arg);
    static void setCLIArgs(int argc, char* argv[]);
    static int argsSize();
    static void setServer(VisuServer* srv);
    static VisuServer* getServer();


private:
    static VisuAppInfo* getInstance();
    static VisuAppInfo* instance;

    bool inEditorMode;
    bool configWrong;
    QStringList configIssues;
    QStringList cliArgs;
    VisuServer* server;
};

#endif // VISUAPPINFO_H
