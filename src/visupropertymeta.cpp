#include "visupropertymeta.h"

const QString VisuPropertyMeta::KEY_MIN = "min";
const QString VisuPropertyMeta::KEY_MAX = "max";
const QString VisuPropertyMeta::KEY_TYPE = "type";
const QString VisuPropertyMeta::KEY_EXTRA = "extra";
const QString VisuPropertyMeta::DELIMITER = ",";

const char* VisuPropertyMeta::TYPES_MAP[12] =
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
    "hidden"
};

QStringList VisuPropertyMeta::getEnumOptions()
{
    return extra.split(VisuPropertyMeta::DELIMITER);
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
