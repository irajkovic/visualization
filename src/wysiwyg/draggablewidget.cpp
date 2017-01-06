#include "wysiwyg/draggablewidget.h"

#include <QDrag>
#include <QApplication>
#include <QMimeData>

void DraggableWidget::setWidget(QWidget* w)
{
    widget = w;
}

void DraggableWidget::mousePressEvent(QMouseEvent * event)
{
    Q_ASSERT(widget != nullptr);
    if (event->button() == Qt::LeftButton
            && widget->geometry().contains(event->pos()))
        {
            dragStartPosition = event->pos();
        }
}

void DraggableWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);

    QMimeData *mimeData = new QMimeData;
    mimeData->setText("data");
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}
