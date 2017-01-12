#include "visuapplication.h"
#include "exceptions/configloadexception.h"
#include "visuconfigloader.h"

#include <QPainter>
#include <QFile>

VisuApplication::VisuApplication(QString path)
{
    loadConfiguration(path);
    setupWindow();
    server = new VisuServer(&configuration);
}

void VisuApplication::loadConfiguration(QString path)
{
    QByteArray xml = VisuConfigLoader::loadXMLFromFile(path);
    configuration.loadFromXML(this, QString(xml));
}

void VisuApplication::setupWindow()
{
    setGeometry(100, 100, configuration.getWidth(), configuration.getHeight());
    setWindowTitle(configuration.getName());
    setBackgroundColor();
}

void VisuApplication::setBackgroundColor()
{
    QColor color = configuration.getBackgroundColor();
    QString stylesheet = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    setStyleSheet(stylesheet);
}

void VisuApplication::run()
{
    server->start();
}
