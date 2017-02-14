#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include <QMap>
#include "visuwidget.h"
#include "visusignal.h"

class VisuWidgetFactory
{
public:
    static VisuWidget* createWidget(QWidget* parent,
                                    QString type,
                                    const QVector<QPointer<VisuSignal> > &signalsList);
    static VisuWidget* createWidget(QWidget* parent,
                                    QString type,
                                    QMap<QString, QString> properties,
                                    QMap<QString, VisuPropertyMeta> metaProperties,
                                    const QVector<QPointer<VisuSignal> > &signalList);
};

#endif // VISUWIDGETFACTORY_H
