#include "button.h"

const QString Button::TAG_NAME = "BUTTON";

void Button::sendCommand()
{
    mSocket.writeDatagram(
                cActionMessage.toStdString().c_str(),
                cActionMessage.size(),
                mAddress,
                cActionPort);

    emit(widgetActivated(this));
}

void Button::setupButton(QWidget* parent)
{
    setGeometry(QRect(cX, cY, cWidth, cHeight));
    mButton = new QPushButton(parent);
    mButton->setGeometry(QRect(cX, cY, cWidth, cHeight));

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

void Button::loadProperties(QMap<QString, QString> properties)
{
    VisuWidget::loadProperties(properties);

    GET_PROPERTY(cActionIp, QString, properties);
    GET_PROPERTY(cActionPort, quint16, properties);
    GET_PROPERTY(cActionMessage, QString, properties);
    GET_PROPERTY(cCss, QString, properties);
}

void Button::redraw()
{
    mButton->setText(cName);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setMinimumSize(cWidth, cHeight);
    mButton->setStyleSheet(cCss);
}
