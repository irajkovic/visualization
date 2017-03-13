#include "visupropertymeta.h"

const QString VisuPropertyMeta::KEY_MIN = "min";
const QString VisuPropertyMeta::KEY_MAX = "max";
const QString VisuPropertyMeta::KEY_TYPE = "type";
const QString VisuPropertyMeta::KEY_EXTRA = "extra";
const QString VisuPropertyMeta::KEY_LABEL = "label";
const QString VisuPropertyMeta::KEY_DEPENDS = "depends";
const QString VisuPropertyMeta::DELIMITER = ",";
#include <QtCore>
const char* VisuPropertyMeta::TYPES_MAP[] =
{
    "",
    "enum",
    "color",
    "signal",
    "read_only",
    "int",
    "float",
    "slider",
    "bool",
    "font",
    "image",
    "serial",
    "serial_placeholder",
    "hidden"
};

QStringList VisuPropertyMeta::getEnumOptions()
{
    return extra.split(VisuPropertyMeta::DELIMITER);
}

bool VisuPropertyMeta::isEnabled(const QMap<QString, QString>& properties) const
{
    bool ret = true;
    int pos;

    if ((pos = depends.indexOf("==")) != -1)
    {
        ret = properties[depends.mid(0, pos)].toInt() == depends.mid(pos + 2).toInt();
    }
    else if ((pos = depends.indexOf("!=")) != -1)
    {
        ret = properties[depends.mid(0, pos)].toInt() != depends.mid(pos + 2).toInt();
    }
    else if ((pos = depends.indexOf('>')) != -1)
    {
        ret = properties[depends.mid(0, pos)].toInt() > depends.mid(pos + 1).toInt();
    }
    if ((pos = depends.indexOf('<')) != -1)
    {
        ret = properties[depends.mid(0, pos)].toInt() < depends.mid(pos + 1).toInt();
    }

    return ret;
}

QString VisuPropertyMeta::stringFromType(VisuPropertyMeta::Type type)
{
    return VisuPropertyMeta::TYPES_MAP[type];
}

VisuPropertyMeta::Type VisuPropertyMeta::typeFromString(QString typeStr)
{
    VisuPropertyMeta::Type ret = VisuPropertyMeta::DEFAULT;
    const char** const begin = std::begin(VisuPropertyMeta::TYPES_MAP);
    const char** const end = std::end(VisuPropertyMeta::TYPES_MAP);
    const char** it = std::find_if( begin,
                             end,
                             [&typeStr](const char* ptr) { return typeStr == ptr; } );


    if (it != end)
    {
        ret = (VisuPropertyMeta::Type)(it - begin);
    }

     return ret;
}
