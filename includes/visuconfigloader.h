#ifndef VISUCONFIGLOADER_H
#define VISUCONFIGLOADER_H

#include <QMap>
#include <QXmlStreamReader>
#include <QSharedPointer>
#include "visupropertymeta.h"

class VisuConfigLoader
{
public:
    VisuConfigLoader();

    static QByteArray loadXMLFromFile(QString path);
    static QMap<QString, VisuPropertyMeta> parseMetaToMap(QXmlStreamReader& xmlReader, QString element);
    static QMap<QString, QString> parseToMap(QXmlStreamReader& xmlReader, QString element);
    static QMap<QString, QString> getMapFromFile(QString type, QString tag);
    static QMap<QString, VisuPropertyMeta> getMetaMapFromFile(QString type, QString tag);

    static const QString PATH;

};

#endif // VISUCONFIGLOADER_H
