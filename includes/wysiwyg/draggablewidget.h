#ifndef DRAGGABLEWIDGET_H
#define DRAGGABLEWIDGET_H


#include <QMouseEvent>
#include <QWidget>

#include "visuwidget.h"

class DraggableWidget : public QWidget
{
public:
    DraggableWidget(QWidget* parent) : QWidget(parent)
    {
        widget = nullptr;
    }

    void setWidget(VisuWidget* w);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    VisuWidget* widget;
    QPoint dragStartPosition;
};

#endif // DRAGGABLEWIDGET_H
