#include "visupropertymeta.h"

const QString VisuPropertyMeta::KEY_MIN = "min";
const QString VisuPropertyMeta::KEY_MAX = "max";
const QString VisuPropertyMeta::KEY_TYPE = "type";
const QString VisuPropertyMeta::KEY_EXTRA = "extra";
const QString VisuPropertyMeta::TYPE_ENUM = "enum";
const QString VisuPropertyMeta::TYPE_COLOR = "color";
const QString VisuPropertyMeta::TYPE_SIGNAL = "signal";
const QString VisuPropertyMeta::TYPE_READ_ONLY = "read_only";
const QString VisuPropertyMeta::TYPE_INT = "int";
const QString VisuPropertyMeta::TYPE_FLOAT = "float";
const QString VisuPropertyMeta::TYPE_BOOL = "bool";

QStringList VisuPropertyMeta::getEnumOptions()
{
    return extra.split(",");
}
