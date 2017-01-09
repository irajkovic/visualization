#include "visuwidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

void VisuWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton
            && geometry().contains(event->pos()))
        {
            mDragStartPosition = event->pos();
        }
}

void VisuWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - mDragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);

    // encode widget type and origin (toolbar / stage)
    QMimeData *mimeData = new QMimeData;
    QString text = cName + "|" + parent()->objectName();
    mimeData->setText(text);
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}
