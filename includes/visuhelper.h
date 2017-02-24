#ifndef VISU_HELPER_H
#define VISU_HELPER_H
#include "exceptions/configloadexception.h"
#include <QColor>
#include <QImage>
#include "visupropertymeta.h"

#define GET_PROPERTY(KEY, TYPE, MAP, METAMAP)  KEY = VisuHelper::get<TYPE>(VisuHelper::transformKey(#KEY), MAP, METAMAP)

namespace VisuHelper
{
    QString transformKey(QString key);
    void checkIfKeyExists(QString key, QMap<QString, QString> properties);
    bool keyMissing(QString key, QMap<QString, QString> properties);

    // handles all integer numeric types
    template <typename T>
    T get(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties)
    {
        if (keyMissing(key, properties))
        {
            properties[key] = metaProperties[key].defaultVal;
        }
        return (T)properties[key].toLongLong();
    }

    // specializations below
    template<>
    double get<double>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties);

    template<>
    QString get<QString>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties);

    template<>
    QColor get<QColor>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties);

    template<>
    QImage get<QImage>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties);

    template<>
    bool get<bool>(QString key, QMap<QString, QString>& properties, QMap<QString, VisuPropertyMeta> metaProperties);
}


#endif // VISU_HELPER_H
