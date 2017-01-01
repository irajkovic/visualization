#ifndef VISUWIDGET_H
#define VISUWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include "visu_helper.h"

class VisuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VisuWidget(
            QWidget *parent,
            QMap<QString, QString> properties) : QWidget(parent)
    {
        this->mProperties = properties;

        // custom properties initializer
        GET_PROPERTY(id, quint16);
        GET_PROPERTY(name, QString);
        GET_PROPERTY(x, quint16);
        GET_PROPERTY(y, quint16);
        GET_PROPERTY(width, quint16);
        GET_PROPERTY(height, quint16);
    }

protected:

    // properties map
    QMap<QString, QString> mProperties;

    quint16 id;
    QString name;
    quint16 x;                  // x position in pixels
    quint16 y;                  // y position in pixels
    quint16 width;              // width in pixels
    quint16 height;             // height in pixels
};

#endif // VISUWIDGET_H
