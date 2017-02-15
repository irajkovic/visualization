#ifndef VISUPROPERTYMETA_H
#define VISUPROPERTYMETA_H

#include <QString>
#include <QStringList>

class VisuPropertyMeta
{
public:

    VisuPropertyMeta() :    min(std::numeric_limits<int>::min()),
                            max(std::numeric_limits<int>::max()),
                            defaultVal(0),
                            type(TYPE_DEFAULT),
                            extra("") {}

    double min;
    double max;
    double defaultVal;
    QString type;
    QString extra;

    QStringList getEnumOptions();

    static const QString DELIMITER;
    static const QString KEY_MIN;
    static const QString KEY_MAX;
    static const QString KEY_TYPE;
    static const QString KEY_EXTRA;
    static const QString TYPE_ENUM;
    static const QString TYPE_COLOR;
    static const QString TYPE_SIGNAL;
    static const QString TYPE_READ_ONLY;
    static const QString TYPE_INT;
    static const QString TYPE_FLOAT;
    static const QString TYPE_SLIDER;
    static const QString TYPE_BOOL;
    static const QString TYPE_FONT;
    static const QString TYPE_DEFAULT;
};

#endif // VISUPROPERTYMETA_H
