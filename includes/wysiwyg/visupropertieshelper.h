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
#include <QLabel>
#include <QCheckBox>

class VisuPropertiesHelper
{
public:
    VisuPropertiesHelper();

    static void updateTable(QTableWidget* table,
                            const QMap<QString, QString>& properties,
                            const QMap<QString, VisuPropertyMeta>& metaProperties,
                            std::pair<QWidget*, const char*> object);
    static void setupTableWidget(  QTableWidget* table,
                                   std::pair<QWidget*, const char*> widget,
                                   std::pair<QWidget*, const char*> parent,
                                   QString key,
                                   VisuPropertyMeta::Type type,
                                   int row);
    static int updateWidgetProperty(QObject* sender, QWidget* parent);
    static void updateWidgetsState(QTableWidget* table,
                                   const QMap<QString, QString>& properties,
                                   const QMap<QString, VisuPropertyMeta>& propertiesMeta);
    static QString getValueString(QTableWidget *table, int row);
    static QString getKeyString(QTableWidget* table, int row);
    static double sliderToDouble(int slider);
    static int doubleToSlider(double value);

    static std::pair<QComboBox*, const char*> setupEnumWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QComboBox*, const char*> setupFontWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QCheckBox*, const char*> setupBoolWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QComboBox*, const char*> setupSignalsWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QComboBox*, const char*> setupImagesWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QComboBox*, const char*> setupSerialWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QComboBox*, const char*> setupSignalPlaceholderWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QPushButton*, const char*> setupColorWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QSpinBox*, const char*> setupIntWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QSlider*, const char*> setupSliderWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QLineEdit*, const char*> setupDefaultWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QLabel*, const char*> setupReadOnlyWidget(VisuPropertyMeta meta, QString value);
    static std::pair<QWidget*, const char*> controlFactory(VisuPropertyMeta meta, QString value);

    static const int SLIDER_FACTOR = 100;
    static const int COLUMN_PROPERTY = 0;
    static const int COLUMN_VALUE = 1;
    static const char* PROP_COLOR;
    static const char* PROP_ROW;
    static const char* PROP_KEY;
    static const char* PROP_TYPE;
};

#endif // VISUPROPERTIESHELPER_H
