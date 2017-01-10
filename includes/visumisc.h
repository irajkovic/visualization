#ifndef VISUMISC_H
#define VISUMISC_H

#include <QWidget>
#include <QColor>
#include <QTableWidget>
#include <QMap>

class VisuMisc
{
public:
    static void setBackgroundColor(QWidget* widget, QColor color);
    static void updateTable(QTableWidget* table, QMap<QString, QString> properties);
};

#endif // VISUMISC_H
