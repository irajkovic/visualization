#include "visuconfigloader.h"
#include <QFile>
#include "exceptions/configloadexception.h"
#include <QXmlStreamAttribute>

const QString VisuConfigLoader::PATH = "system/";

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

QMap<QString, VisuPropertyMeta>* VisuConfigLoader::parseMetaToMap(QXmlStreamReader& xmlReader, QString element)
{
    QMap<QString, VisuPropertyMeta>* map = new QMap<QString, VisuPropertyMeta>;
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
                if (attr.name().toString() == "min")
                {
                    meta.min = attr.value().toDouble();
                }
                else if (attr.name().toString() == "max")
                {
                    meta.max = attr.value().toDouble();
                }
                else if (attr.name().toString() == "type")
                {
                    meta.type = attr.value().toString();
                }
                else if (attr.name().toString() == "extra")
                {
                    meta.extra = attr.value().toString();
                }
            }
        }
        else if (xmlReader.tokenType() == QXmlStreamReader::Characters && !xmlReader.isWhitespace())
        {
            meta.defaultVal = xmlReader.text().toDouble();
            (*map)[name] = meta;
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

QMap<QString, VisuPropertyMeta>* VisuConfigLoader::getMetaMapFromFile(QString type, QString tag)
{
    QString path = PATH + type + ".xml";
    QString xmlString = VisuConfigLoader::loadXMLFromFile(path);
    QXmlStreamReader xmlReader(xmlString);
    return VisuConfigLoader::parseMetaToMap(xmlReader, tag);
}
