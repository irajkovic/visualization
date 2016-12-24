#include "visu_helper.h"

#include "exceptions/configloadexception.h"

#define GET_PROPERTY(KEY, TYPE)  KEY = VisuHelper::get<TYPE>(#KEY, properties)

namespace VisuHelper
{
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
}
