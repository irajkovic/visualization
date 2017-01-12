#include "../includes/statics/staticimage.h"

#include <QLabel>
#include <QDir>
const QString StaticImage::TAG_NAME = "IMAGE";

void StaticImage::renderImage()
{
    QLabel* label = new QLabel(this);
    setGeometry(QRect(cX, cY, cWidth, cHeight));
    label->setPixmap(QPixmap::fromImage(cData));
    label->show();
    show();
}
