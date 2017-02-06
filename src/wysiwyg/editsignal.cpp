#include "wysiwyg/editsignal.h"
#include "wysiwyg/visupropertieshelper.h"
#include "visusignal.h"

void EditSignal::setup(QPointer<VisuSignal> visuSignal)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Signal parameters"));

    QLayout* vlayout = new QVBoxLayout();
    setLayout(vlayout);

    mSignal = visuSignal;
    mTable = new QTableWidget() ;

    mMetaProperties = VisuConfigLoader::getMetaMapFromFile(VisuSignal::TAG_NAME, VisuSignal::TAG_NAME);

    if (visuSignal == nullptr)
    {
        mProperties = VisuConfigLoader::getMapFromFile(VisuSignal::TAG_NAME, VisuSignal::TAG_NAME);
        mSignal = new VisuSignal(mProperties);
        mNewSignal = true;
    }
    else
    {
        mProperties = visuSignal->getProperties();
        mNewSignal = false;
    }
    VisuPropertiesHelper::updateTable(mTable, mProperties, mMetaProperties, nullptr, this, SLOT(propertyChange()));

    mTable->setMaximumWidth(300);
    mTable->verticalHeader()->hide();
    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    vlayout->addWidget(mTable);

    QWidget* buttons = new QWidget();
    QLayout* buttonsLayout = new QHBoxLayout();
    buttons->setLayout(buttonsLayout);

    QPushButton* saveButton = new QPushButton(tr("&Save"));
    buttonsLayout->addWidget(saveButton);

    QPushButton* cancelButton = new QPushButton(tr("&Cancel"));
    buttonsLayout->addWidget(cancelButton);

    buttons->setMaximumHeight(saveButton->height());
    vlayout->addWidget(buttons);

    show();
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(addSignal()));
}

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
    QString value = VisuPropertiesHelper::getValueString(mTable, row);
    mProperties[key] = value;
    mSignal->load(mProperties);
}

void EditSignal::propertyChange()
{
    int row = VisuPropertiesHelper::updateWidgetProperty(sender(), this);
    cellUpdated(row, 1);
}

