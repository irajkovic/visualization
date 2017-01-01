#include "button.h"

const QString Button::TAG_NAME = "BUTTON";

void Button::sendCommand()
{
    mSocket.writeDatagram(
                action_message.toStdString().c_str(),
                action_message.size(),
                mAddress,
                action_port);
}

void Button::setupButton(QWidget* parent)
{
    mButton = new QPushButton(parent);
    mButton->setGeometry(QRect(x, y, width, height));
    mButton->setText(label);
    mButton->setStyleSheet(css);
    QObject::connect(mButton,
                     SIGNAL(pressed()),
                     this,
                     SLOT(sendCommand()));
}
