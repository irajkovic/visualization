#include "button.h"

const QString Button::TAG_NAME = "BUTTON";

void Button::sendCommand()
{
    mSocket.writeDatagram(
                message.toStdString().c_str(),
                message.size(),
                mAddress,
                action_port);
}
