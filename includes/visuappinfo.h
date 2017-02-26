#ifndef VISUAPPINFO_H
#define VISUAPPINFO_H

#include <QString>
#include <QStringList>

class VisuAppInfo
{

public:
    static bool isInEditorMode();
    static void setInEditorMode(bool mode);

    static bool isConfigWrong();
    static void setConfigWrong(bool wrong);
    static void setConfigWrong(bool wrong, const QString& issue);
    static const QStringList& getConfigIssues();

private:
    static VisuAppInfo* getInstance();
    static VisuAppInfo* instance;

    bool inEditorMode;
    bool configWrong;
    QStringList configIssues;
};

#endif // VISUAPPINFO_H
