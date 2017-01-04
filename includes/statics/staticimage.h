#ifndef STATICIMAGE_H
#define STATICIMAGE_H

#include "visuwidget.h"

class StaticImage : public VisuWidget
{
public:
    explicit StaticImage(
            QWidget *parent,
            QMap<QString, QString> properties) : VisuWidget(parent, properties)
    {

        GET_PROPERTY(cData, QImage, properties);

        renderImage();
    }
    static const QString TAG_NAME;

private:
    QImage cData;

    void renderImage();
};

#endif // STATICIMAGE_H
