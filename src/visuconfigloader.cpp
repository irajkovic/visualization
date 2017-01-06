#include "visuconfigloader.h"
#include <QFile>
#include "exceptions/configloadexception.h"

VisuConfigLoader::VisuConfigLoader()
{
}

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

QMap<QString, QString> VisuConfigLoader::parseToMap(QXmlStreamReader& xmlReader, QString element)
{
    QMap<QString, QString> map;
    QString name;
    QString value;

    while ( xmlReader.tokenType() != QXmlStreamReader::EndElement
             || xmlReader.name() != element) {

        if (xmlReader.tokenType() == QXmlStreamReader::Invalid) {
            QString errorMsg = xmlReader.errorString() + " Near node: \"%1\"";
            throw ConfigLoadException(errorMsg, xmlReader.name().toString());
        }

        if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
            name = xmlReader.name().toString();
        }

        if (xmlReader.tokenType() == QXmlStreamReader::Characters
                && !xmlReader.isWhitespace()) {
            value = xmlReader.text().toString();

            map[name] = value;

            //DBG_XML;
        }

        xmlReader.readNext();

    }

    return map;

}

QMap<QString, QString> VisuConfigLoader::getTagFromFile(QString file, QString tag)
{
    QString xmlString = VisuConfigLoader::loadXMLFromFile(file);
    QXmlStreamReader xmlReader(xmlString);
    return VisuConfigLoader::parseToMap(xmlReader, tag);
}

