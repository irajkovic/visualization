#include "button.h"

const QString Button::TAG_NAME = "BUTTON";

void Button::sendCommand()
{
    mSocket.writeDatagram(
                cActionMessage.toStdString().c_str(),
                cActionMessage.size(),
                mAddress,
                cActionPort);
}

void Button::setupButton(QWidget* parent)
{
    mButton = new QPushButton(parent);
    mButton->setGeometry(QRect(cX, cY, cWidth, cHeight));
    mButton->setText(cName);
    mButton->setStyleSheet(cCss);
    QObject::connect(mButton,
                     SIGNAL(pressed()),
                     this,
                     SLOT(sendCommand()));
}
