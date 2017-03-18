#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "visuappinfo.h"
#include "visuserver.h"
#include "visuapplication.h"
#include "exceptions/configloadexception.h"

#define DEFAULT_CONFIG "configs/default.xml"

void showMessageBox(QString message)
{
    QMessageBox::warning(
                NULL,
                "Error",
                message);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VisuAppInfo::setCLIArgs(argc, argv);
    try
    {
        if (argc == 1)
        {
            VisuAppInfo::setInEditorMode(true);
            new MainWindow();
        }
        else
        {
            QString configPath = VisuAppInfo::getCLIArg(VisuAppInfo::CLI_Args::CONFIG_PATH);
            VisuApplication *application = new VisuApplication(configPath);
            application->show();
            application->run();
        }
    }
    catch(ConfigLoadException e)
    {
        showMessageBox(e.what());
        return 1;
    }
    catch(...)
    {
        showMessageBox("Unknown exception!");
        return 1;
    }

    return a.exec();
}
