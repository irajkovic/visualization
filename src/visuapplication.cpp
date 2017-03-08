#include "visuapplication.h"
#include "exceptions/configloadexception.h"
#include "visuconfigloader.h"
#include "visumisc.h"
#include <QPainter>
#include <QFile>

VisuApplication::VisuApplication(QString path)
{
    mConfiguration = VisuConfiguration::get();
    loadConfiguration(path);
    setupWindow();
    mServer = new VisuServer();
}

void VisuApplication::loadConfiguration(QString path)
{
    QByteArray xml = VisuConfigLoader::loadXMLFromFile(path);
    mConfiguration->fromXML(this, QString(xml));
}

void VisuApplication::setupWindow()
{
    setGeometry(100, 100, mConfiguration->getWidth(), mConfiguration->getHeight());
    setWindowTitle(mConfiguration->getName());
    VisuMisc::setBackgroundColor(this, mConfiguration->getBackgroundColor());
}

void VisuApplication::run()
{
    mServer->start();
}
