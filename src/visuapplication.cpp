#include "visuapplication.h"
#include "exceptions/configloadexception.h"

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
    QByteArray xml = loadXMLFromFile(path);
    configuration.loadFromXML(this, QString(xml));
}

QByteArray VisuApplication::loadXMLFromFile(QString path)
{
    QFile xml_file(path);
    xml_file.open(QFile::ReadOnly);
    QByteArray contents = xml_file.readAll();

    if (contents.isEmpty())
    {
        throw ConfigLoadException("Error loading config from file %1", path);
    }
    return contents;
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
