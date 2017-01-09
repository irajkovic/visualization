#include "visuwidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

#define DEBUG_STR(EVENT) qDebug(EVENT": %s", objectName().toStdString().c_str());

const QString VisuWidget::OBJECT_NAME = "VisuWidget";

void VisuWidget::mousePressEvent(QMouseEvent * event)
{
    DEBUG_STR("mousePressEvent");
    if (event->button() == Qt::LeftButton && geometry().contains(event->pos()))
    {
        mDragStartPosition = event->pos();
    }
    event->ignore();
}

void VisuWidget::mouseReleaseEvent(QMouseEvent* event)
{
    DEBUG_STR("mouseReleaseEvent");
    event->ignore();
}

void VisuWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    DEBUG_STR("mouseDoubleClickEvent");
    event->ignore();
}

void VisuWidget::mouseMoveEvent(QMouseEvent *event)
{
    DEBUG_STR("mouseMoveEvent");
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - mDragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    drag->setPixmap(QPixmap::grabWidget(this));

    // encode widget type and origin (toolbar / stage)
    QMimeData *mimeData = new QMimeData;
    QString text = mTagName + "|" + parent()->objectName();
    mimeData->setText(text);
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
    event->ignore();
}

