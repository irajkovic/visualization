#ifndef VISUPROPERTIESHELPER_H
#define VISUPROPERTIESHELPER_H

#include <QPalette>
#include <QPushButton>
#include <QColorDialog>
#include <QTextStream>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>
#include "visupropertymeta.h"
#include "visusignal.h"
#include <QSharedPointer>

class VisuPropertiesHelper
{
public:
    VisuPropertiesHelper();

    static void updateTable(QTableWidget* table,
                            QMap<QString, QString> properties,
                            QMap<QString, VisuPropertyMeta> metaProperties,
                            QVector<QPointer<VisuSignal>>* signalList = nullptr,
                            QWidget* object = nullptr,
                            const char* slot = nullptr);
    static void setupTableWidget(QWidget* widget,
                                    QTableWidget* table,
                                    QWidget* object,
                                    const char* slot,
                                    QString key,
                                    int row);
    static int updateWidgetProperty(QObject* sender, QWidget* parent);
    static QString getValueString(QTableWidget *table, int row);
    static double sliderToDouble(int slider);
    static int doubleToSlider(double value);

    static const int SLIDER_FACTOR = 100;
    static const char* PROP_COLOR;
    static const char* PROP_ROW;
    static const char* PROP_KEY;
    static const char* PROP_X;
    static const char* PROP_Y;
    static const char* PROP_SIGNAL_ID;
    static const char* PROP_ID;
};

#endif // VISUPROPERTIESHELPER_H
