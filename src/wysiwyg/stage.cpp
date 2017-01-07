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
/*
    if (instrument == "analog")
    {
        VisuSignal* signal = mMainWindow->getSignal();
        CREATE_INSTRUMENT_OBJECT(this, InstAnalog, analog, signal);
        //InstAnalog* analog = new InstAnalog(this, VisuConfigLoader::getTagFromFile("system/analog.xml", "instrument"));
        analog->setPosition(event->pos());
        //signal->connectInstrument(analog);

        analog->show();

        signal->initialUpdate();
    }
*/


    //qDebug("%s", .toStdString().c_str());
    event->acceptProposedAction();
}
