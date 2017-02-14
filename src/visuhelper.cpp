#include "visuhelper.h"
#include "exceptions/configloadexception.h"
#include "statics/staticimage.h"
#include <QByteArray>

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

    template<>
    double get<double>(QString key, QMap<QString, QString> properties)
    {
        checkIfKeyExists(key, properties);
        return properties[key].toDouble();
    }

    template<>
    QString get<QString>(QString key, QMap<QString, QString> properties)
    {
        checkIfKeyExists(key, properties);
        return properties[key];
    }

    template<>
    QColor get<QColor>(QString key, QMap<QString, QString> properties)
    {
        checkIfKeyExists(key, properties);
        QStringList parts = properties[key].split(",");
        if (parts.length() == 4) {
            return QColor( parts[0].toInt()
                         , parts[1].toInt()
                         , parts[2].toInt()
                         , parts[3].toInt());
        }
        else
        {
            throw ConfigLoadException("Wrong color format (%1)", properties[key]);
        }
    }

    template<>
    QImage get<QImage>(QString key, QMap<QString, QString> properties)
    {
        checkIfKeyExists(key, properties);
        checkIfKeyExists(StaticImage::KEY_FORMAT, properties);

        QByteArray base64Bytes;
        base64Bytes.append(properties[key]);

        QByteArray rawImageData;
        rawImageData = QByteArray::fromBase64(base64Bytes);

        QImage image;
        image.loadFromData(rawImageData, properties[StaticImage::KEY_FORMAT].toStdString().c_str());
        return image;
    }

    template<>
    bool get<bool>(QString key, QMap<QString, QString> properties)
    {
        checkIfKeyExists(key, properties);
        return (properties[key].toInt() != 0);
    }
}
