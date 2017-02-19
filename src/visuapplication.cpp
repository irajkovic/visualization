#include "visuapplication.h"
#include "exceptions/configloadexception.h"
#include "visuconfigloader.h"
#include "visumisc.h"
#include <QPainter>
#include <QFile>

VisuApplication::VisuApplication(QString path)
{
    configuration = VisuConfiguration::get();
    loadConfiguration(path);
    setupWindow();
    server = new VisuServer(configuration);
}

void VisuApplication::loadConfiguration(QString path)
{
    QByteArray xml = VisuConfigLoader::loadXMLFromFile(path);
    configuration->fromXML(this, QString(xml));
}

void VisuApplication::setupWindow()
{
    setGeometry(100, 100, configuration->getWidth(), configuration->getHeight());
    setWindowTitle(configuration->getName());
    VisuMisc::setBackgroundColor(this, configuration->getBackgroundColor());
}

void VisuApplication::run()
{
    server->start();
}
