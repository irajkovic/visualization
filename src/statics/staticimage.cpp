#include "../includes/statics/staticimage.h"

#include <QLabel>
#include <QDir>
#include <QPainter>
#include <QStyleOption>

const QString StaticImage::TAG_NAME = "IMAGE";
const QString StaticImage::KEY_FORMAT = "format";
const QString StaticImage::KEY_IMAGE = "image";

void StaticImage::renderImage()
{
    QLabel* label = new QLabel();
    mLayout->addWidget(label);

    setGeometry(QRect(cX, cY, cWidth, cHeight));

    label->setPixmap(QPixmap::fromImage(cImage));
    label->show();
    show();


}

void StaticImage::paintEvent(QPaintEvent* event)
{
    (void)event;    // supress compiler warning about unused parameter
    QPainter painter(this);

    // allow for custom stylesheets
    QStyleOption option;
    option.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
