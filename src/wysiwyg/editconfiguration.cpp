#include "wysiwyg/editconfiguration.h"

void EditConfiguration::updateConfigParams()
{
    emit(configParamsUpdated());
    close();
}

void EditConfiguration::cellUpdated(int row, int col)
{
    QString key = mTable->item(row, 0)->text();
    QString value = mTable->item(row, 1)->text();
    mProperties[key] = value;
    mConfiguration->setConfigValues(mProperties);
}
