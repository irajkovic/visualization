#ifndef VISUMISC_H
#define VISUMISC_H

#include <QWidget>
#include <QColor>
#include <QTableWidget>
#include <QMap>
#include <QFile>

class VisuMisc
{
public:
    static bool isColorProperty(QString property);
    static void setBackgroundColor(QWidget* widget, QColor color);
    static void updateTable(QTableWidget* table,
                            QMap<QString, QString> properties,
                            QWidget* object = nullptr,
                            const char* slot = nullptr);
    static QString mapToString(QMap<QString, QString> properties, int tabs);
    static int updateColor(QObject* sender, QWidget* parent);
    static QString getValueString(int row, QString key, QTableWidget *table);
    static QString saveToFile(QFile &file, QString contents);

    static const char* PROP_COLOR;
    static const char* PROP_ROW;
    static const char* PROP_X;
    static const char* PROP_Y;
    static const char* PROP_SIGNAL_ID;
};

#endif // VISUMISC_H
