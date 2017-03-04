#include "visuconfigloader.h"
#include <QFile>
#include "exceptions/configloadexception.h"
#include <QXmlStreamAttribute>
#include "visuappinfo.h"

const QString VisuConfigLoader::PATH = "system/";
const QString VisuConfigLoader::ORDER = "_order";
const QString VisuConfigLoader::ORDER_DELIMITER = ",";

QByteArray VisuConfigLoader::loadXMLFromFile(QString path)
{
    QFile xml_file(path);
    xml_file.open(QFile::ReadOnly);
    QByteArray contents = xml_file.readAll();
    xml_file.close();

    if (contents.isEmpty())
    {
        throw ConfigLoadException("Error loading config from file %1", path);
    }
    return contents;
}

QMap<QString, VisuPropertyMeta> VisuConfigLoader::parseMetaToMap(QXmlStreamReader& xmlReader, QString element)
{
    QMap<QString, VisuPropertyMeta> map;
    QString name;
    VisuPropertyMeta meta;

    while (xmlReader.tokenType() != QXmlStreamReader::EndElement || xmlReader.name() != element)
    {
        if (xmlReader.tokenType() == QXmlStreamReader::Invalid)
        {
            QString errorMsg = xmlReader.errorString() + " Near node: \"%1\"";
            throw ConfigLoadException(errorMsg, xmlReader.name().toString());
        }
        else if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
        {
            name = xmlReader.name().toString();
            meta = VisuPropertyMeta();

            for (const QXmlStreamAttribute& attr : xmlReader.attributes())
            {
                if (attr.name().toString() == VisuPropertyMeta::KEY_MIN)
                {
                    meta.min = attr.value().toDouble();
                }
                else if (attr.name().toString() == VisuPropertyMeta::KEY_MAX)
                {
                    meta.max = attr.value().toDouble();
                }
                else if (attr.name().toString() == VisuPropertyMeta::KEY_TYPE)
                {
                    meta.type = VisuPropertyMeta::typeFromString(attr.value().toString());
                }
                else if (attr.name().toString() == VisuPropertyMeta::KEY_EXTRA)
                {
                    meta.extra = attr.value().toString();
                }
                else if (attr.name().toString() == VisuPropertyMeta::KEY_LABEL)
                {
                    meta.label = attr.value().toString();
                }
            }
        }
        else if (xmlReader.tokenType() == QXmlStreamReader::Characters && !xmlReader.isWhitespace())
        {
            meta.defaultVal = xmlReader.text().toString();
            map[name] = meta;
        }

        xmlReader.readNext();
    }

    return map;

}

QMap<QString, QString> VisuConfigLoader::parseToMap(QXmlStreamReader& xmlReader, QString element)
{
    QMap<QString, QString> map;
    QString name;
    QString value;

    bool trackInsertionOrder = VisuAppInfo::isInEditorMode();
    if (trackInsertionOrder)
    {
        map[ORDER] = "";
    }

    while ( xmlReader.tokenType() != QXmlStreamReader::EndElement || xmlReader.name() != element)
    {

        if (xmlReader.tokenType() == QXmlStreamReader::Invalid)
        {
            QString errorMsg = xmlReader.errorString() + " Near node: \"%1\"";
            throw ConfigLoadException(errorMsg, xmlReader.name().toString());
        }
        else if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
        {
            name = xmlReader.name().toString();
        }
        else if (xmlReader.tokenType() == QXmlStreamReader::Characters && !xmlReader.isWhitespace())
        {
            value = xmlReader.text().toString();
            map[name] = value;

            if (trackInsertionOrder)
            {
                map[ORDER].append(QString("%1%2").arg(name).arg(ORDER_DELIMITER));
            }
        }

        xmlReader.readNext();

    }

    return map;

}

QMap<QString, QString> VisuConfigLoader::getMapFromFile(QString type, QString tag)
{
    QString path = PATH + type + ".xml";
    QString xmlString = VisuConfigLoader::loadXMLFromFile(path);
    QXmlStreamReader xmlReader(xmlString);
    return VisuConfigLoader::parseToMap(xmlReader, tag);
}

QMap<QString, VisuPropertyMeta> VisuConfigLoader::getMetaMapFromFile(QString type, QString tag)
{
    QString path = PATH + type + ".xml";
    QString xmlString = VisuConfigLoader::loadXMLFromFile(path);
    QXmlStreamReader xmlReader(xmlString);
    return VisuConfigLoader::parseMetaToMap(xmlReader, tag);
}
