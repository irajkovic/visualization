#include "wysiwyg/stage.h"

#include <QMimeData>
#include "instruments/instanalog.h"
#include "visuconfigloader.h"
#include "wysiwyg/visuwidgetfactory.h"
#include "ctrlbutton.h"

void Stage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void Stage::dropEvent(QDropEvent *event)
{
    VisuWidget* widget;
    VisuWidget* sourceWidget = static_cast<VisuWidget*>(event->source());

    if (mMainWindow->dragOriginIsToolbar(sourceWidget))
    {
        // This is new widget
        widget = cloneWidget(sourceWidget);
        connect(widget, SIGNAL(widgetActivated(VisuWidget*)), this, SLOT(activateWidget(VisuWidget*)));
        mMainWindow->updateMenuWidgetsList();
    }
    else
    {
        widget = sourceWidget;
    }

    QPoint position = getNewWidgetPosition(event->pos(),
                                           sourceWidget->getRelativeOffset(),
                                           sourceWidget->size());

    widget->setPosition(position);
    mMainWindow->setActiveWidget(widget);
    mMainWindow->setChanged();
    event->acceptProposedAction();
}

VisuWidget* Stage::cloneWidget(VisuWidget *sourceWidget)
{    
    VisuWidget* widget = VisuWidgetFactory::createWidget(this, sourceWidget->getProperties());
    mMainWindow->getConfiguration()->addWidget(widget);
    widget->show();
    widget->refresh(VisuWidget::KEY_ID);
    return widget;
}

QPoint Stage::getNewWidgetPosition(QPoint eventPos, QPoint grabOffset, QSize instSize)
{
    QPoint position = eventPos - grabOffset;

    // corect if horizontal position outside of container
    if (position.x() < 0)
    {
        position.setX(0);
    }
    else if (position.x() + instSize.width() > width())
    {
        position.setX(width() - instSize.width());
    }

    // corect if vertical position outside of container
    if (position.y() < 0)
    {
        position.setY(0);
    }
    else if (position.y() + instSize.height() > height())
    {
        position.setY(height() - instSize.height());
    }

    return position;
}

void Stage::activateWidget(VisuWidget* widget)
{
    mMainWindow->setActiveWidget(widget);
}

void Stage::paintEvent(QPaintEvent *event)
{
    // Allow stylesheets
    (void)event;
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

QSize Stage::sizeHint() const
{
    VisuConfiguration* config = mMainWindow->getConfiguration();
    return (config != nullptr) ? config->getSize() : QSize();
}
