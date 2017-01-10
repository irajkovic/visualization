#include "wysiwyg/editsignal.h"

#include "visusignal.h"

void EditSignal::addSignal()
{
    VisuSignal* signal = new VisuSignal(mProperties);
    emit(signalAdded(signal));
    close();
}

void EditSignal::cellUpdated(int row, int col)
{
    QString key = mTable->item(row, 0)->text();
    QString value = mTable->item(row, 1)->text();
    mProperties[key] = value;
    qDebug("%s %s", key.toStdString().c_str(), value.toStdString().c_str());
}
