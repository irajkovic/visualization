#include "../includes/statics/staticimage.h"

#include <QLabel>
#include <QDir>
#include <QPainter>
#include <QStyleOption>

const QString StaticImage::TAG_NAME = "IMAGE";
const QString StaticImage::KEY_FORMAT = "format";
const QString StaticImage::KEY_IMAGE = "image";

bool StaticImage::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    StaticImage::loadProperties();
    return StaticImage::refresh(key);
}

void StaticImage::loadProperties()
{
    VisuWidget::loadProperties();
    GET_PROPERTY(cImage, mProperties, mPropertiesMeta);
    GET_PROPERTY(cShow, mProperties, mPropertiesMeta);
    GET_PROPERTY(cResize, mProperties, mPropertiesMeta);
}

void StaticImage::paintEvent(QPaintEvent* event)
{
    (void)event;    // supress compiler warning about unused parameter

    if (!cShow)
    {
        return;
    }

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

bool StaticImage::refresh(const QString& key)
{
    VisuWidget::refresh(key);
    setVisible(cShow);
    return false;
}

QImage StaticImage::getImage()
{
    return cImage;
}
