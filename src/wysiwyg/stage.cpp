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
        // TODO :: Refactor
        QString path = QString("system/%1.xml").arg(type);
        QString xmlString = VisuConfigLoader::loadXMLFromFile(path);
        QXmlStreamReader xmlReader(xmlString);
        widget = mMainWindow->getConfiguration()->createInstrumentFromToken(xmlReader, this);

        VisuSignal* signal = mMainWindow->getSignal();
        signal->initializeInstruments();

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

void Stage::paintEvent(QPaintEvent *event)
{
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
