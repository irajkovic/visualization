#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include <QMap>
#include "visuwidget.h"
#include "visusignal.h"

class VisuWidgetFactory
{
public:
    VisuWidgetFactory();
    static VisuWidget* createInstrument(QWidget* parent, QString type, VisuSignal* signal = nullptr);
    static VisuWidget* createInstrument(QWidget* parent,
                                    QString type,
                                    QMap<QString, QString> properties,
                                    VisuSignal* signal = nullptr);
    static VisuWidget* createControl(QWidget* parent, QString type);
    static VisuWidget* createControl(QWidget* parent,
                                     QMap<QString, QString> properties);
};

#endif // VISUWIDGETFACTORY_H
