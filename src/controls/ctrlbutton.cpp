#include "ctrlbutton.h"

const QString CtrlButton::TAG_NAME = "BUTTON";

void CtrlButton::sendCommand()
{
    mSocket.writeDatagram(
                cActionMessage.toStdString().c_str(),
                cActionMessage.size(),
                mAddress,
                cActionPort);

    emit(widgetActivated(this));
}

void CtrlButton::setupButton(QWidget* parent)
{
    setGeometry(QRect(cX, cY, cWidth, cHeight));
    mButton = new QPushButton(parent);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setMinimumSize(cWidth, cHeight);

    mLayout->addWidget(mButton);
    mButton->setText(cName);
    mButton->setStyleSheet(cCss);
    QObject::connect(mButton,
                     SIGNAL(pressed()),
                     this,
                     SLOT(sendCommand()));
}

void CtrlButton::reloadProperties(QMap<QString, QString>& properties)
{
    CtrlButton::loadProperties(properties);
}

void CtrlButton::loadProperties(QMap<QString, QString>& properties)
{
    VisuControl::loadProperties(properties);

    GET_PROPERTY(cActionMessage, QString, properties, mPropertiesMeta);
    GET_PROPERTY(cCss, QString, properties, mPropertiesMeta);
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

bool CtrlButton::refresh(const QString& key)
{
    VisuWidget::refresh(key);
    mButton->setText(cName);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setStyleSheet(cCss);
    return false;
}
