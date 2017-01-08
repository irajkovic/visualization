#include "wysiwyg/stage.h"

#include <QMimeData>
#include "instruments/instanalog.h"
#include "visuconfigloader.h"
#include "wysiwyg/visuwidgetfactory.h"

void Stage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void Stage::dropEvent(QDropEvent *event)
{
    QString type = event->mimeData()->text();
    VisuSignal* signal = mMainWindow->getSignal();

    VisuWidget* widget = VisuWidgetFactory::createWidget(this, signal, type);
    widget->setPosition(event->pos());

    mMainWindow->setPropertiesTable(widget);

    event->acceptProposedAction();
}
