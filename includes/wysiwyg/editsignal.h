#ifndef EDITSIGNAL_H
#define EDITSIGNAL_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include "visumisc.h"
#include "visuconfigloader.h"
#include "visusignal.h"
#include <QTableWidget>
#include <QMap>

class EditSignal : public QWidget
{
    Q_OBJECT

public:
    EditSignal(VisuSignal* visuSignal = nullptr)
    {
        QLayout* vlayout = new QVBoxLayout();
        setLayout(vlayout);

        mSignal = visuSignal;
        mTable = new QTableWidget() ;

        if (visuSignal == nullptr)
        {
            mProperties = VisuConfigLoader::getTagFromFile("system/signal.xml", "signal");
            mSignal = new VisuSignal(mProperties);
            mNewSignal = true;
        }
        else
        {
            mProperties = visuSignal->getProperties();
            mNewSignal = false;
        }
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
        connect(saveButton, SIGNAL(clicked()), this, SLOT(addSignal()));
        connect(mTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
    }

signals:
    void signalAdded(VisuSignal*,bool);

public slots:
    void addSignal();
    void cellUpdated(int row, int col);

private:
    QMap<QString, QString> mProperties;
    QTableWidget* mTable;
    bool mNewSignal;
    VisuSignal* mSignal;
};

#endif // EDITSIGNAL_H
