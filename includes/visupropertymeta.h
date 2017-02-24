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
                            type(DEFAULT),
                            extra("") {}

    typedef enum
    {
        DEFAULT,
        ENUM,
        COLOR,
        INSTSIGNAL,
        READ_ONLY,
        INT,
        FLOAT,
        SLIDER,
        BOOL,
        FONT,
        IMAGE,
        HIDDEN,
        FIRST = DEFAULT,
        LAST = HIDDEN
    } Type;
    static const char* TYPES_MAP[12];

    double min;
    double max;
    double defaultVal;
    Type type;
    QString extra;

    QStringList getEnumOptions();
    static Type typeFromString(QString typeStr);

    static const QString DELIMITER;
    static const QString KEY_MIN;
    static const QString KEY_MAX;
    static const QString KEY_TYPE;
    static const QString KEY_EXTRA;




};

#endif // VISUPROPERTYMETA_H
