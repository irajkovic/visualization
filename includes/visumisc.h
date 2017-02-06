#ifndef VISUMISC_H
#define VISUMISC_H

#include <QWidget>
#include <QColor>
#include <QTableWidget>
#include <QMap>
#include <QFile>
#include <visuconfiguration.h>
#include "visupropertymeta.h"

class VisuMisc
{
public:
    static void setBackgroundColor(QWidget* widget, QColor color);
    static QString getXMLDeclaration();
    static QString openTag(QString tag, int tabs = 0);
    static QString closeTag(QString tag, int tabs = 0);
    static QString mapToString(QMap<QString, QString> properties, int tabs = 0);
    static QString saveToFile(QFile &file, QString contents);


};

#endif // VISUMISC_H
