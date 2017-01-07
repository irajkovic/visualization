#ifndef VISUWIDGET_H
#define VISUWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include "visuhelper.h"

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
        GET_PROPERTY(cId, quint16, properties);
        GET_PROPERTY(cName, QString, properties);
        GET_PROPERTY(cX, quint16, properties);
        GET_PROPERTY(cY, quint16, properties);
        GET_PROPERTY(cWidth, quint16, properties);
        GET_PROPERTY(cHeight, quint16, properties);
    }

    QString getName()
    {
        return cName;
    }
    void setName(QString name)
    {
        cName = name;
    }
    void setPosition(QPoint position)
    {
        setGeometry(position.x(), position.y(), cWidth, cHeight);
    }

protected:

    // properties map
    QMap<QString, QString> mProperties;

    quint16 cId;
    QString cName;
    quint16 cX;                  // x position in pixels
    quint16 cY;                  // y position in pixels
    quint16 cWidth;              // width in pixels
    quint16 cHeight;             // height in pixels
};

#endif // VISUWIDGET_H
