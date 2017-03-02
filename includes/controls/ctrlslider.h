#ifndef CTRLSLIDER_H
#define CTRLSLIDER_H

#include <QHBoxLayout>
#include <QSlider>
#include "visucontrol.h"

class CtrlSlider : public VisuControl
{
    Q_OBJECT

public:
    explicit CtrlSlider(
            QWidget *parent,
            QMap<QString, QString> properties,
            QMap<QString, VisuPropertyMeta> metaProperties) : VisuControl(parent, properties, metaProperties)
    {

        loadProperties();
        setup(parent);
    }

    static const QString TAG_NAME;
    static const QString KEY_HORIZONTAL;

    virtual bool updateProperties(const QString &key, const QString &value);
    void loadProperties();
    virtual bool refresh(const QString& key);
    void setup(QWidget* parent);

    private:

    bool cHorizontal;
    quint32 cRange;

    // styling
    QColor cColorBackground;
    QColor cColorForeground;
    QColor cColorBorder;
    quint32 cBorderRadius;
    quint16 cFontSize;
    QString cFontType;
    quint8 cBorderThickness;
    QString cCss;

    QSlider* mSlider;
    QHBoxLayout* mLayout;

    void setupSlider(QWidget* parent);
    QString generateCss();

    private slots:
    void sendCommand(int value);
};

#endif // CTRLSLIDER_H
