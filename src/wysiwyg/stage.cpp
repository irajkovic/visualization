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

QString getType(QString mimeDataText)
{
    int delPos = mimeDataText.indexOf("\|");
    return mimeDataText.mid(0, delPos);
}

void Stage::dropEvent(QDropEvent *event)
{

    QStringList parts = event->mimeData()->text().split("|");
    QString type = parts[0];
    QString origin = parts[1];

    qDebug("%s", event->mimeData()->text().toStdString().c_str());

    VisuWidget* widget;
    if (mMainWindow->dragOriginIsToolbar(origin))
    {
        VisuSignal* signal = mMainWindow->getSignal();
        widget = VisuWidgetFactory::createWidget(this, type, signal);
        widget->show();
        connect(widget, SIGNAL(widgetActivated(VisuWidget*)), this, SLOT(activateWidget(VisuWidget*)));
    }
    else
    {
        widget = mMainWindow->getActiveWidget();
    }

    VisuWidget* sourceWidget = static_cast<VisuWidget*>(event->source());
    widget->setPosition(event->pos() - sourceWidget->getRelativeOffset());

    mMainWindow->setActiveWidget(widget);

    event->acceptProposedAction();
}

void Stage::activateWidget(VisuWidget* widget)
{
    mMainWindow->setActiveWidget(widget);
}

