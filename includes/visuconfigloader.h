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
    static QMap<QString, VisuPropertyMeta>* parseMetaToMap(QXmlStreamReader& xmlReader, QString element);
    static QMap<QString, QString> parseToMap(QXmlStreamReader& xmlReader, QString element);
    static QMap<QString, QString> getMapFromFile(QString file, QString tag);
    static QMap<QString, VisuPropertyMeta> *getMetaMapFromFile(QString file, QString tag);

};

#endif // VISUCONFIGLOADER_H
