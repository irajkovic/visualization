#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include <QMap>
#include "visuwidget.h"
#include "visusignal.h"

class VisuWidgetFactory
{
public:
    VisuWidgetFactory();
    static VisuWidget* createWidget(QWidget* parent, QString type, VisuSignal* signal = nullptr);
    static VisuWidget* createWidget(QWidget* parent,
                                    QString type,
                                    QMap<QString, QString> properties,
                                    VisuSignal* signal = nullptr);
};

#endif // VISUWIDGETFACTORY_H
