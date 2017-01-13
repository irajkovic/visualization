#include "wysiwyg/editsignal.h"

#include "visusignal.h"

void EditSignal::addSignal()
{
    mSignal->initializeInstruments();
    emit(signalAdded(mSignal,mNewSignal));
    close();
}

void EditSignal::cellUpdated(int row, int col)
{
    (void)col;
    QString key = mTable->item(row, 0)->text();
    QString value = mTable->item(row, 1)->text();
    mProperties[key] = value;
    mSignal->load(mProperties);
}
