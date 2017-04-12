#ifndef VISU_HELPER_H
#define VISU_HELPER_H
#include "exceptions/configloadexception.h"
#include <QColor>
#include <QImage>
#include "visupropertymeta.h"

#define GET_PROPERTY(KEY, MAP, METAMAP) \
    VisuPropertyLoader::set( KEY, \
                             VisuPropertyLoader::transformKey(#KEY),\
                             MAP, \
                             METAMAP)

namespace VisuPropertyLoader
{
    QString transformKey(const QString& key);
    void checkIfKeyExists(QString key, QMap<QString, QString> properties);
    void handleMissingKey(const QString& key,
                          QMap<QString, QString>& properties,
                          const QMap<QString, VisuPropertyMeta>& metaProperties);

    // handles all integer numeric types
    template <typename T>
    void set(  T& property,
          const QString& key,
          QMap<QString, QString>& properties,
          const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);
        property = (T)properties[key].toLongLong();
    }

    // specializations below
    void set( double& property,
              const QString& key,
              QMap<QString, QString>& properties,
              const QMap<QString, VisuPropertyMeta>& metaProperties);

    void set( QString& property,
              const QString& key,
              QMap<QString, QString>& properties,
              const QMap<QString, VisuPropertyMeta>& metaProperties);


    void set( QColor& property,
              const QString& key,
              QMap<QString, QString>& properties,
              const QMap<QString, VisuPropertyMeta>& metaProperties);


    void set( QImage& property,
              const QString& key,
              QMap<QString, QString>& properties,
              const QMap<QString, VisuPropertyMeta>& metaProperties);


    void set( bool& property,
              const QString& key,
              QMap<QString, QString>& properties,
              const QMap<QString, VisuPropertyMeta>& metaProperties);
}


#endif // VISU_HELPER_H
