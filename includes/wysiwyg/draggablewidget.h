#ifndef DRAGGABLEWIDGET_H
#define DRAGGABLEWIDGET_H


#include <QMouseEvent>
#include <QWidget>

class DraggableWidget : public QWidget
{
public:
    DraggableWidget(QWidget* parent) : QWidget(parent)
    {
        widget = nullptr;
    }

    void setWidget(QWidget* w);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QWidget* widget;
    QPoint dragStartPosition;
};

#endif // DRAGGABLEWIDGET_H
