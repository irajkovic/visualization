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
        load(properties);
        setObjectName(VisuWidget::OBJECT_NAME);
    }

    virtual void load(QMap<QString, QString> properties)
    {
        mProperties = properties;

        // custom properties initializer
        GET_PROPERTY(cId, quint16, properties);
        GET_PROPERTY(cName, QString, properties);
        GET_PROPERTY(cX, quint16, properties);
        GET_PROPERTY(cY, quint16, properties);
        GET_PROPERTY(cWidth, quint16, properties);
        GET_PROPERTY(cHeight, quint16, properties);

        mSize = QSize(cWidth, cHeight);
        setMinimumSize(mSize);
        setMaximumSize(mSize);
    }

    QMap<QString, QString> getProperties()
    {
        return mProperties;
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
        cX = position.x();
        cY = position.y();
        setGeometry(cX, cY, cWidth, cHeight);
        mProperties["x"] = QString("%1").arg(cX);
        mProperties["y"] = QString("%1").arg(cY);
    }
    const QSize sizeHint()
    {
        return mSize;
    }

    // Drag&drop related
    QPoint mDragStartPosition;
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

    static const QString OBJECT_NAME;

protected:

    // properties map
    QMap<QString, QString> mProperties;

    quint16 cId;
    QString cName;
    quint16 cX;                  // x position in pixels
    quint16 cY;                  // y position in pixels
    quint16 cWidth;              // width in pixels
    quint16 cHeight;             // height in pixels

    QSize mSize;
};

#endif // VISUWIDGET_H
