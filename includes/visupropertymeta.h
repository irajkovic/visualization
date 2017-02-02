#ifndef VISUPROPERTYMETA_H
#define VISUPROPERTYMETA_H

struct VisuPropertyMeta
{
    VisuPropertyMeta() : min(0), max(0), defaultVal(0), type("") {}

    double min;
    double max;
    double defaultVal;
    QString type;
    QString extra;
};

#endif // VISUPROPERTYMETA_H
