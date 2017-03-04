#include "visupropertymeta.h"

const QString VisuPropertyMeta::KEY_MIN = "min";
const QString VisuPropertyMeta::KEY_MAX = "max";
const QString VisuPropertyMeta::KEY_TYPE = "type";
const QString VisuPropertyMeta::KEY_EXTRA = "extra";
const QString VisuPropertyMeta::KEY_LABEL = "label";
const QString VisuPropertyMeta::DELIMITER = ",";

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
    "hidden"
};

QStringList VisuPropertyMeta::getEnumOptions()
{
    return extra.split(VisuPropertyMeta::DELIMITER);
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
