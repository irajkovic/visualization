#ifndef VISU_HELPER_H
#define VISU_HELPER_H
#include "exceptions/configloadexception.h"
#include <QColor>
#include <QImage>
#include "visupropertymeta.h"

#define GET_PROPERTY(KEY, TYPE, MAP, METAMAP) \
    KEY = VisuPropertyConverter::get<TYPE>(VisuPropertyConverter::transformKey(#KEY), MAP, METAMAP)

namespace VisuPropertyConverter
{
    QString transformKey(const QString& key);
    void checkIfKeyExists(QString key, QMap<QString, QString> properties);
    void handleMissingKey(const QString& key,
                          QMap<QString, QString>& properties,
                          const QMap<QString, VisuPropertyMeta>& metaProperties);

    // handles all integer numeric types
    template <typename T>
    T get(const QString& key,
          QMap<QString, QString>& properties,
          const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);
        return (T)properties[key].toLongLong();
    }

    // specializations below
    template<>
    double get<double>(const QString& key,
                       QMap<QString, QString>& properties,
                       const QMap<QString, VisuPropertyMeta>& metaProperties);

    template<>
    QString get<QString>(const QString& key,
                         QMap<QString, QString>& properties,
                         const QMap<QString, VisuPropertyMeta>& metaProperties);

    template<>
    QColor get<QColor>(const QString& key,
                       QMap<QString, QString>& properties,
                       const QMap<QString, VisuPropertyMeta>& metaProperties);

    template<>
    QImage get<QImage>(const QString& key,
                       QMap<QString, QString>& properties,
                       const QMap<QString, VisuPropertyMeta>& metaProperties);

    template<>
    bool get<bool>(const QString& key,
                   QMap<QString, QString>& properties,
                   const QMap<QString, VisuPropertyMeta>& metaProperties);
}


#endif // VISU_HELPER_H
