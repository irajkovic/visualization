#include "visuhelper.h"
#include "exceptions/configloadexception.h"
#include "statics/staticimage.h"
#include <QByteArray>
#include "visumisc.h"

namespace VisuHelper
{
    QString transformKey(QString key)
    {
        return key.mid(1, 1).toLower() + key.mid(2);
    }

    void checkIfKeyExists(QString key, QMap<QString, QString> properties)
    {
        if (!properties.contains(key))
        {
            throw ConfigLoadException("Missing property: %1", key);
        }
    }

    bool keyMissing(QString key, QMap<QString, QString> properties)
    {
        if (!properties.contains(key))
        {
            qDebug("Missing property: %s", key.toStdString().c_str());
            return true;
        }
        return false;
    }

    template<>
    double get<double>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties)
    {
        if (keyMissing(key, properties))
        {
            properties[key] = metaProperties[key].defaultVal;
        }
        return properties[key].toDouble();
    }

    template<>
    QString get<QString>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties)
    {
        if (keyMissing(key, properties))
        {
            properties[key] =  metaProperties[key].defaultVal;
        }
        return properties[key];
    }

    template<>
    QColor get<QColor>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties)
    {
        if (keyMissing(key, properties))
        {
            properties[key] = metaProperties[key].defaultVal;
        }

        QColor color = VisuMisc::strToColor(properties[key]);
        if (!color.isValid())
        {
            throw ConfigLoadException("Wrong color format (%1)", properties[key]);
        }
        return color;
    }

    template<>
    QImage get<QImage>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties)
    {
        if (keyMissing(key, properties))
        {
            properties[key] = metaProperties[key].defaultVal;
        }

        return VisuMisc::strToImage(properties[key], properties[StaticImage::KEY_FORMAT]);
    }

    template<>
    bool get<bool>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties)
    {
        if (keyMissing(key, properties))
        {
            properties[key] = metaProperties[key].defaultVal;
        }
        return (properties[key].toInt() != 0);
    }
}
