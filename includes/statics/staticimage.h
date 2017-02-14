#ifndef STATICIMAGE_H
#define STATICIMAGE_H

#include "visuwidget.h"
#include <QHBoxLayout>

class StaticImage : public VisuWidget
{
    Q_OBJECT

public:
    explicit StaticImage(
            QWidget *parent,
            QMap<QString, QString> properties) : VisuWidget(parent, properties)
    {

        GET_PROPERTY(cImage, QImage, properties);
        mTagName = StaticImage::TAG_NAME;

        mLayout = new QHBoxLayout();
        mLayout->setContentsMargins(0,0,0,0);
        setLayout(mLayout);

        renderImage();
    }

    static const QString TAG_NAME;
    static const QString KEY_FORMAT;
    static const QString KEY_IMAGE;

    void paintEvent(QPaintEvent* event);

private:
    QImage cImage;

    QHBoxLayout* mLayout;

    void renderImage();
};

#endif // STATICIMAGE_H
