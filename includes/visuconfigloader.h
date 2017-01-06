#ifndef VISUCONFIGLOADER_H
#define VISUCONFIGLOADER_H

#include <QMap>
#include <QXmlStreamReader>

class VisuConfigLoader
{
public:
    VisuConfigLoader();

    static QByteArray loadXMLFromFile(QString path);
    static QMap<QString, QString> parseToMap(QXmlStreamReader& xmlReader, QString element);
    static QMap<QString, QString> getTagFromFile(QString file, QString tag);

};

#endif // VISUCONFIGLOADER_H
