#ifndef VISUAPPINFO_H
#define VISUAPPINFO_H

class VisuAppInfo
{

public:
    static bool isInEditorMode();
    static void setInEditorMode(bool mode);

    static bool isConfigWrong();
    static void setConfigWrong(bool wrong);

private:
    static VisuAppInfo* getInstance();

    static VisuAppInfo* instance;
    bool inEditorMode;
    bool configWrong;
};

#endif // VISUAPPINFO_H
