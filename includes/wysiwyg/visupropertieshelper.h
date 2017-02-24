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
#include <QSpinBox>
#include <QSlider>

class VisuPropertiesHelper
{
public:
    VisuPropertiesHelper();

    static void updateTable(QTableWidget* table,
                            QMap<QString, QString> properties,
                            QMap<QString, VisuPropertyMeta> metaProperties,
                            QWidget* object = nullptr,
                            const char* slot = nullptr);
    static void setupTableWidget(QTableWidget* table, QWidget* widget,
                                    QWidget* object, const char *signal,
                                    const char* slot,
                                    QString key,
                                    VisuPropertyMeta::Type type,
                                    int row);
    static int updateWidgetProperty(QObject* sender, QWidget* parent);
    static QString getValueString(QTableWidget *table, int row);
    static double sliderToDouble(int slider);
    static int doubleToSlider(double value);

    static QComboBox* setupEnumWidget(VisuPropertyMeta meta, QString value);
    static QComboBox* setupFontWidget(VisuPropertyMeta meta, QString value);
    static QComboBox* setupBoolWidget(VisuPropertyMeta meta, QString value);
    static QComboBox* setupSignalsWidget(VisuPropertyMeta meta, QString value);
    static QComboBox* setupImagesWidget(VisuPropertyMeta meta, QString value);
    static QPushButton* setupColorWidget(VisuPropertyMeta meta, QString value, QColor color);
    static QSpinBox* setupIntWidget(VisuPropertyMeta meta, QString value);
    static QSlider* setupSliderWidget(VisuPropertyMeta meta, QString value);
    static QLineEdit* setupDefaultWidget(VisuPropertyMeta meta, QString value);
    static void setupReadOnlyWidget(QTableWidget* table, int row, QString value);


    static const int SLIDER_FACTOR = 100;
    static const char* PROP_COLOR;
    static const char* PROP_ROW;
    static const char* PROP_KEY;
    static const char* PROP_TYPE;
};

#endif // VISUPROPERTIESHELPER_H
