#include "../includes/statics/staticimage.h"

#include <QLabel>
#include <QDir>
#include <QPainter>
#include <QStyleOption>

const QString StaticImage::TAG_NAME = "IMAGE";
const QString StaticImage::KEY_FORMAT = "format";
const QString StaticImage::KEY_IMAGE = "image";

void StaticImage::loadProperties(QMap<QString, QString> properties)
{
    VisuWidget::loadProperties(properties);
    GET_PROPERTY(cImage, QImage, properties);
    GET_PROPERTY(cShow, bool, properties);
    GET_PROPERTY(cResize, bool, properties);
}

void StaticImage::paintEvent(QPaintEvent* event)
{
    (void)event;    // supress compiler warning about unused parameter
    QPainter painter(this);

    if (cResize)
    {
        painter.drawImage(QRectF(0, 0, cWidth, cHeight),
                          cImage,
                          QRectF(0, 0, cImage.width(), cImage.height()));
    }
    else
    {
        painter.drawImage(0, 0, cImage, 0, 0);
    }

    drawActiveBox(&painter);
}

void StaticImage::refresh(const QString& key)
{
    VisuWidget::refresh(key);
    setVisible(cShow);
}
