#ifndef VISUWIDGET_H
#define VISUWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include "visuhelper.h"
#include "visupropertymeta.h"

class VisuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VisuWidget(
            QWidget *parent,
            QMap<QString, QString> properties) : QWidget(parent), mPropertiesMeta(nullptr)
    {
        loadProperties(properties);
        setObjectName(VisuWidget::OBJECT_NAME);
    }

    virtual void loadProperties(QMap<QString, QString> properties);

    QMap<QString, QString> getProperties();
    void setPropertiesMeta(QMap<QString, VisuPropertyMeta>* meta);
    QMap<QString, VisuPropertyMeta>* getPropertiesMeta();
    QString getName();
    void setName(QString name);
    void setPosition(QPoint position);
    const QSize sizeHint();
    QString getType();
    void setActive(bool active);
    quint16 getId() const;
    void setId(quint16 id);

    // Drag&drop related
    QPoint mDragStartPosition;
    QPoint mDragStartRelativePosition;
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    QPoint getRelativeOffset();

    static const QString OBJECT_NAME;

signals:
    void widgetActivated(VisuWidget*);

protected:

    // properties map
    QMap<QString, QString> mProperties;
    QMap<QString, VisuPropertyMeta>* mPropertiesMeta;

    quint16 cId;
    QString cName;
    quint16 cX;                  // x position in pixels
    quint16 cY;                  // y position in pixels
    quint16 cWidth;              // width in pixels
    quint16 cHeight;             // height in pixels

    QSize mSize;
    QString mTagName;
    bool    mActive;        // true when widget is active in wysiwyg editor
};

#endif // VISUWIDGET_H
