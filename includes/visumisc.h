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
    static bool isIdProperty(QString property);
    static void setBackgroundColor(QWidget* widget, QColor color);
    static void updateTable(QTableWidget* table,
                            QMap<QString, QString> properties,
                            QMap<QString, VisuPropertyMeta>* metaProperties = nullptr,
                            QVector<QPointer<VisuSignal>>* signalList = nullptr,
                            QWidget* object = nullptr,
                            const char* slot = nullptr);
    static QString mapToString(QMap<QString, QString> properties, int tabs);
    static int updateWidgetProperty(QObject* sender, QWidget* parent);
    static QString getValueString(int row, QString key, QTableWidget *table);
    static QString saveToFile(QFile &file, QString contents);
    static void setupTableWidget(QWidget* widget,
                                    QTableWidget* table,
                                    QWidget* object,
                                    const char* slot,
                                    QString key,
                                    int row);

    static const char* PROP_COLOR;
    static const char* PROP_ROW;
    static const char* PROP_KEY;
    static const char* PROP_X;
    static const char* PROP_Y;
    static const char* PROP_SIGNAL_ID;
    static const char* PROP_ID;
};

#endif // VISUMISC_H
