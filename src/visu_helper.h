#ifndef VISU_HELPER_H
#define VISU_HELPER_H
#include "exceptions/configloadexception.h"
#include <QColor>

#define GET_PROPERTY(KEY, TYPE)  KEY = VisuHelper::get<TYPE>(#KEY, properties)

namespace VisuHelper
{
    void checkIfKeyExists(QString key, QMap<QString, QString> properties);

    // handles all integer numeric types
    template <typename T>
    T get(QString key, QMap<QString, QString> properties)
    {
        checkIfKeyExists(key, properties);
        return (T)properties[key].toLongLong();
    }

    // specializations below
    template<>
    double get<double>(QString key, QMap<QString, QString> properties);

    template<>
    QString get<QString>(QString key, QMap<QString, QString> properties);

    template<>
    QColor get<QColor>(QString key, QMap<QString, QString> properties);
}


#endif // VISU_HELPER_H
