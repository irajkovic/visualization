#include "ctrlbutton.h"
#include "visumisc.h"
#include <visuserver.h>

const QString CtrlButton::TAG_NAME = "BUTTON";

void CtrlButton::sendCommand()
{
    if (VisuConfiguration::get()->getConectivity() != VisuServer::SERIAL_ONLY)
    {
        mSocket.writeDatagram(
                    cActionMessage.toStdString().c_str(),
                    cActionMessage.size(),
                    mAddress,
                    cActionPort);
    }

    if (VisuConfiguration::get()->getConectivity() != VisuServer::UDP_ONLY)
    {
        VisuServer* server = VisuAppInfo::getServer();
        if (server != nullptr)
        {
            server->sendSerial(cActionMessage.toLatin1());
        }
    }

    emit(widgetActivated(this));
}

void CtrlButton::setupButton(QWidget* parent)
{
    setGeometry(QRect(cX, cY, cWidth, cHeight));
    mButton = new QPushButton(parent);
    mLayout->addWidget(mButton);

    refresh("");

    QObject::connect(mButton,
                     SIGNAL(pressed()),
                     this,
                     SLOT(sendCommand()));
}

bool CtrlButton::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    CtrlButton::loadProperties();
    return CtrlButton::refresh(key);
}

void CtrlButton::loadProperties()
{
    VisuControl::loadProperties();

    GET_PROPERTY(cActionMessage, mProperties, mPropertiesMeta);
    GET_PROPERTY(cCss, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorBackground, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorForeground, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorBorder, mProperties, mPropertiesMeta);
    GET_PROPERTY(cBorderRadius, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFontSize, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFontType, mProperties, mPropertiesMeta);
    GET_PROPERTY(cBorderThickness, mProperties, mPropertiesMeta);
}

void CtrlButton::setup(QWidget *parent)
{
    mTagName = TAG_NAME;
    mLayout = new QHBoxLayout();
    mLayout->setContentsMargins(0,0,0,0);
    setLayout(mLayout);
    setupButton(parent);
    show();
}

QString CtrlButton::generateCss()
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

bool CtrlButton::refresh(const QString& key)
{
    VisuWidget::refresh(key);
    mButton->setText(cName);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setStyleSheet(generateCss());
    return false;
}
