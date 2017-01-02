#include "button.h"

const QString Button::TAG_NAME = "BUTTON";

void Button::sendCommand()
{
    mSocket.writeDatagram(
                actionMessage.toStdString().c_str(),
                actionMessage.size(),
                mAddress,
                actionPort);
}

void Button::setupButton(QWidget* parent)
{
    mButton = new QPushButton(parent);
    mButton->setGeometry(QRect(x, y, width, height));
    mButton->setText(name);
    mButton->setStyleSheet(css);
    QObject::connect(mButton,
                     SIGNAL(pressed()),
                     this,
                     SLOT(sendCommand()));
}
