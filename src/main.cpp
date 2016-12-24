#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

#include "server.h"
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

    QString xmlPath = (argc > 1) ? QString(argv[1]) : DEFAULT_CONFIG;

    try
    {
        VisuApplication *application = new VisuApplication(xmlPath);
        application->show();
        application->run();
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
