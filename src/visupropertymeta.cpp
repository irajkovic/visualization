#include "visupropertymeta.h"

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
