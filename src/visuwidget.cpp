#include "visuwidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

const QString VisuWidget::OBJECT_NAME = "VisuWidget";

QPoint VisuWidget::getRelativeOffset()
{
    return mDragStartRelativePosition;
}

void VisuWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && geometry().contains(event->pos()))
    {
        mDragStartPosition = event->pos();
    }

    emit(widgetActivated(this));
}

void VisuWidget::mouseReleaseEvent(QMouseEvent* event)
{

}

void VisuWidget::mouseDoubleClickEvent(QMouseEvent* event)
{

}

void VisuWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - mDragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    drag->setHotSpot(event->pos());
    mDragStartRelativePosition = event->pos();

    drag->setPixmap(grab());

    // encode widget type and origin (toolbar / stage)
    QMimeData *mimeData = new QMimeData;
    QString text = mTagName + "|" + parent()->objectName();
    mimeData->setText(text);
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}
