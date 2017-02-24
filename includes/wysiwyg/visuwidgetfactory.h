#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include <QMap>
#include "visuwidget.h"
#include "visusignal.h"

class VisuWidgetFactory
{
public:
    static VisuWidget* createWidget(QWidget* parent,
                                    QString type);
    static VisuWidget* createWidget(QWidget* parent,
                                    QString type,
                                    QMap<QString, QString> properties);
};

#endif // VISUWIDGETFACTORY_H
