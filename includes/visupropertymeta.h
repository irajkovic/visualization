#ifndef VISUPROPERTYMETA_H
#define VISUPROPERTYMETA_H

#include <QString>
#include <QStringList>

struct VisuPropertyMeta
{
    VisuPropertyMeta() : min(std::numeric_limits<int>::min()), max(std::numeric_limits<int>::max()), defaultVal(0), type("") {}

    double min;
    double max;
    double defaultVal;
    QString type;
    QString extra;

    QStringList getEnumOptions();

    static const QString TYPE_ENUM;
    static const QString TYPE_COLOR;
    static const QString TYPE_SIGNAL;
    static const QString TYPE_READ_ONLY;
    static const QString TYPE_INT;
    static const QString TYPE_FLOAT;
    static const QString TYPE_BOOL;
};

#endif // VISUPROPERTYMETA_H
