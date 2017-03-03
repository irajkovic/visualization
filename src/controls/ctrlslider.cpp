#include "ctrlslider.h"
#include "visumisc.h"

const QString CtrlSlider::TAG_NAME = "SLIDER";
const QString CtrlSlider::KEY_HORIZONTAL = "horizontal";

void CtrlSlider::sendCommand(int value)
{
    QString message = QString("%1").arg(value);
    mSocket.writeDatagram(
                message.toStdString().c_str(),
                message.size(),
                mAddress,
                cActionPort);

    emit(widgetActivated(this));
}

void CtrlSlider::setupSlider(QWidget* parent)
{
    setGeometry(QRect(cX, cY, cWidth, cHeight));
    mSlider = new QSlider(cHorizontal ? Qt::Horizontal : Qt::Vertical);
    mLayout->addWidget(mSlider);

    refresh("");

    QObject::connect(mSlider,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(sendCommand(int)));

}

bool CtrlSlider::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    CtrlSlider::loadProperties();
    return CtrlSlider::refresh(key);
}

void CtrlSlider::loadProperties()
{
    VisuControl::loadProperties();

    GET_PROPERTY(cRange, mProperties, mPropertiesMeta);
    GET_PROPERTY(cCss, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorBackground, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorForeground, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorBorder, mProperties, mPropertiesMeta);
    GET_PROPERTY(cBorderRadius, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFontSize, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFontType, mProperties, mPropertiesMeta);
    GET_PROPERTY(cBorderThickness, mProperties, mPropertiesMeta);
    GET_PROPERTY(cHorizontal, mProperties, mPropertiesMeta);
}

void CtrlSlider::setup(QWidget *parent)
{
    mTagName = TAG_NAME;
    mLayout = new QHBoxLayout();
    mLayout->setContentsMargins(0,0,0,0);
    setLayout(mLayout);
    setupSlider(parent);
    show();
}

QString CtrlSlider::generateCss()
{
    QString css;
    css += QString("background-color: %1;").arg(VisuMisc::colorToStr(cColorBackground));
    css += QString("color: %1;").arg(VisuMisc::colorToStr(cColorForeground));
    css += QString("border: %1px solid %2;").arg(cBorderThickness).arg(VisuMisc::colorToStr(cColorBorder));
    css += QString("border-radius: %1;").arg(cBorderRadius);
    css += QString("font-family: %1;").arg(cFontType);
    css += QString("font-size: %1px;").arg(cFontSize);
    css += cCss;
    return css;
}

bool CtrlSlider::refresh(const QString& key)
{
    bool changed = false;
    if (key == KEY_HORIZONTAL)
    {
        mSlider->setOrientation(cHorizontal ? Qt::Horizontal : Qt::Vertical);
        std::swap(cWidth, cHeight);
        std::swap(mProperties[KEY_WIDTH], mProperties[KEY_HEIGHT]);
        changed = true;
    }

    mSlider->setMinimum(0);
    mSlider->setMaximum(cRange);

    mSlider->setMinimumSize(cWidth, cHeight);
    mSlider->setMinimumSize(cWidth, cHeight);
    mSlider->setStyleSheet(generateCss());

    VisuWidget::refresh(key);
    VisuWidget::setup();

    return changed;
}
