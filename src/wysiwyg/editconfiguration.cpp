#include "wysiwyg/editconfiguration.h"

EditConfiguration::EditConfiguration(QPointer<VisuConfiguration> configuration)
{
    setAttribute(Qt::WA_DeleteOnClose);

    mConfiguration = configuration;

    QLayout* vlayout = new QVBoxLayout();
    setLayout(vlayout);
    mProperties = mConfiguration->getProperties();

    mTable = new QTableWidget();
    VisuMisc::updateTable(mTable, mProperties);
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
    connect(saveButton, SIGNAL(clicked()), this, SLOT(updateConfigParams()));
    connect(mTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
}

void EditConfiguration::updateConfigParams()
{
    emit(configParamsUpdated());
    close();
}

void EditConfiguration::cellUpdated(int row, int col)
{
    (void)col;
    QString key = mTable->item(row, 0)->text();
    QString value = mTable->item(row, 1)->text();
    mProperties[key] = value;
    mConfiguration->setConfigValues(mProperties);
}
