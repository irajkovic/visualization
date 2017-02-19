#include "visuwidget.h"
#include "visumisc.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QStyleOption>
#include <QPainter>

const QString VisuWidget::OBJECT_NAME = "VisuWidget";
const QString VisuWidget::TAG_NAME = "widget";
const QString VisuWidget::KEY_ID = "id";
const QString VisuWidget::KEY_WIDTH = "width";
const QString VisuWidget::KEY_HEIGHT = "height";
const QString VisuWidget::KEY_X = "x";
const QString VisuWidget::KEY_Y = "y";
const QString VisuWidget::KEY_NAME = "name";


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
    else if (event->button() == Qt::RightButton)
    {
        emit(widgetContextMenu(this));
    }

    emit(widgetActivated(this));
}

void VisuWidget::mouseReleaseEvent(QMouseEvent* event)
{
    (void)event;
}

void VisuWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    (void)event;

    emit(widgetActivated(this));
}

void VisuWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        return;
    }
    if ((event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
    {
        return;
    }

    QDrag *drag = new QDrag(this);
    drag->setHotSpot(event->pos());
    mDragStartRelativePosition = event->pos();

    drag->setPixmap(grab());

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(mTagName);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void VisuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOption option;
    option.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void VisuWidget::drawActiveBox(QPainter* painter)
{
    if (mActive)
    {
        QPen pen;
        pen.setColor(Qt::GlobalColor::black);
        pen.setWidth(4);
        painter->setPen(pen);
        painter->drawRect(0, 0, cWidth, cHeight);
        painter->setPen(VisuMisc::getDashedPen(Qt::GlobalColor::white, 4));
        painter->drawRect(0, 0, cWidth, cHeight);
    }
}

void VisuWidget::loadProperties(QMap<QString, QString> properties)
{
    mProperties = properties;

    // custom properties initializer
    GET_PROPERTY(cId, quint16, properties);
    GET_PROPERTY(cName, QString, properties);
    GET_PROPERTY(cX, quint16, properties);
    GET_PROPERTY(cY, quint16, properties);
    GET_PROPERTY(cWidth, quint16, properties);
    GET_PROPERTY(cHeight, quint16, properties);

    mSize = QSize(cWidth, cHeight);
    setMinimumSize(mSize);
    setMaximumSize(mSize);
    mActive = false;
}

quint16 VisuWidget::getId() const
{
    return cId;
}

void VisuWidget::setId(quint16 id)
{
    cId = id;
    mProperties[VisuWidget::KEY_ID] = QString("%1").arg(id);
}

QMap<QString, QString> VisuWidget::getProperties()
{
    return mProperties;
}

void VisuWidget::setPropertiesMeta(QMap<QString, VisuPropertyMeta> meta)
{
    mPropertiesMeta = meta;
}

QMap<QString, VisuPropertyMeta> VisuWidget::getPropertiesMeta()
{
    return mPropertiesMeta;
}

QString VisuWidget::getName()
{
    return cName;
}

void VisuWidget::setName(QString name)
{
    cName = name;
}

void VisuWidget::setPosition(QPoint position)
{
    cX = position.x();
    cY = position.y();
    setGeometry(cX, cY, cWidth, cHeight);
    mProperties[VisuWidget::KEY_X] = QString("%1").arg(cX);
    mProperties[VisuWidget::KEY_Y] = QString("%1").arg(cY);
}

const QSize VisuWidget::sizeHint()
{
    return mSize;
}

QString VisuWidget::getType()
{
    return mTagName;
}

void VisuWidget::setActive(bool active)
{
    mActive = active;
    update();
}

