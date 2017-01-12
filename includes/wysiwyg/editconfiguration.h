#ifndef EDITCLASS_H
#define EDITCLASS_H

#include <QHeaderView>
#include <QWidget>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QMap>
#include "visuconfiguration.h"
#include "visumisc.h"

class EditConfiguration : public QWidget
{
    Q_OBJECT

public:
    EditConfiguration(VisuConfiguration* configuration)
    {
        mConfiguration = configuration;

        QLayout* vlayout = new QVBoxLayout();
        setLayout(vlayout);
        mProperties = mConfiguration->getProperties();

        mTable = new QTableWidget() ;
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

signals:
    void configParamsUpdated();

private slots:
    void updateConfigParams();
    void cellUpdated(int row, int col);

private:
    VisuConfiguration* mConfiguration;
    QMap<QString, QString> mProperties;
    QTableWidget* mTable;
};

#endif // EDITCLASS_H
