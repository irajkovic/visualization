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
        loadProperties(properties);
        mTagName = StaticImage::TAG_NAME;
        setVisible(cShow);
    }

    static const QString TAG_NAME;
    static const QString KEY_FORMAT;
    static const QString KEY_IMAGE;

    virtual void loadProperties(QMap<QString, QString> properties);
    void paintEvent(QPaintEvent* event);
    virtual bool refresh(const QString& key);
    QImage getImage();

private:
    QImage cImage;
    bool cShow;
    bool cResize;
};

#endif // STATICIMAGE_H
