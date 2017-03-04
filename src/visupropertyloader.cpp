#include "visupropertyloader.h"
#include "exceptions/configloadexception.h"
#include "statics/staticimage.h"
#include "visuappinfo.h"
#include <QByteArray>
#include "visumisc.h"

namespace VisuPropertyLoader
{
    QString transformKey(const QString &key)
    {
        return key.mid(1, 1).toLower() + key.mid(2);
    }

    void handleMissingKey(const QString &key,
                          QMap<QString, QString>& properties,
                          const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        if (!properties.contains(key))
        {
            ConfigLoadException exception(QObject::tr("Missing property: %1 (%2)").arg(metaProperties[key].label).arg(key));
            VisuAppInfo::setConfigWrong(true, exception.getMessage());

            if (VisuAppInfo::isInEditorMode())
            {
                properties[key] = metaProperties[key].defaultVal;
            }
            else
            {
                throw exception;
            }
        }
    }

    void set(double& property,
                       const QString& key,
                       QMap<QString, QString>& properties,
                       const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);
        property = properties[key].toDouble();
    }


    void set(QString& property,
                         const QString& key,
                         QMap<QString, QString>& properties,
                         const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);
        property = properties[key];
    }


    void set(QColor& property,
                       const QString& key,
                       QMap<QString, QString>& properties,
                       const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);

        QColor color = VisuMisc::strToColor(properties[key]);
        if (!color.isValid())
        {
            throw ConfigLoadException("Wrong color format (%1)", properties[key]);
        }
        property = color;
    }


    void set(QImage& property,
                       const QString& key,
                       QMap<QString, QString>& properties,
                       const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);
        property = VisuMisc::strToImage(properties[key], properties[StaticImage::KEY_FORMAT]);
    }

    void set(bool& property,
                   const QString& key,
                   QMap<QString, QString>& properties,
                   const QMap<QString, VisuPropertyMeta>& metaProperties)
    {
        handleMissingKey(key, properties, metaProperties);
        property = (properties[key].toInt() != 0);
    }
}
