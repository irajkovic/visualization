#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "visuapplication.h"
#include "instanalog.h"
#include "instdigital.h"
#include "instlinear.h"
#include "insttimeplot.h"
#include "instled.h"
#include "visuconfigloader.h"
#include "wysiwyg/visuwidgetfactory.h"
#include "wysiwyg/draggablewidget.h"
#include <QXmlStreamReader>
#include <QTableWidget>
#include "wysiwyg/stage.h"
#include <QLabel>

#include <QLineEdit>
#include <QTableWidgetItem>

MainWindow::MainWindow(QString xmlPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* window = new QWidget();
    QVBoxLayout* windowLayout = new QVBoxLayout();
    window->setLayout(windowLayout);
    setCentralWidget(window);

    QWidget* toolbar = new QWidget(window);
    toolbar->setMinimumHeight(170);
    windowLayout->addWidget(toolbar);
    setupToolbarWidgets(toolbar);

    QWidget* workArea = new QWidget(window);
    windowLayout->addWidget(workArea);

    QHBoxLayout* workAreaLayout = new QHBoxLayout();
    workArea->setLayout(workAreaLayout);

    Stage* stage = new Stage(this, workArea);
    workAreaLayout->addWidget(stage);

    mPropertiesTable = new QTableWidget(workArea);
    mPropertiesTable->setMaximumWidth(300);
    mPropertiesTable->verticalHeader()->hide();
    mPropertiesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    workAreaLayout->addWidget(mPropertiesTable);


    stage->setStyleSheet("background-color: gray;");
#if 0
    toolbar->setStyleSheet("background-color: green;");
    workArea->setStyleSheet("background-color: gray;");
#endif
    setWindowState(Qt::WindowMaximized);
    show();
}

void MainWindow::setupToolbarWidgets(QWidget* toolbar)
{
    QHBoxLayout *layout = new QHBoxLayout;
    toolbar->setLayout(layout);

    mSignal = new VisuSignal(VisuConfigLoader::getTagFromFile("system/signal.xml", "signal"));

    ADD_INSTRUMENT_TO_LAYOUT(this, InstAnalog, analog, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstLinear, linear, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstTimePlot, timeplot, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstDigital, digital, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstLED, led, mSignal, layout);

    mSignal->initialUpdate();
}

void MainWindow::cellUpdated(int row, int col)
{
    QString key = mPropertiesTable->item(row,0)->text();
    QString value = mPropertiesTable->item(row,1)->text();

    QMap<QString, QString> properties = mActiveWidget->getProperties();
    QPoint position = mActiveWidget->pos();

    if (key == "x")
    {
        position.setX(value.toInt());
    }
    else if (key == "y")
    {
        position.setY(value.toInt());
    }

    properties[key] = value;
    mActiveWidget->load(properties);
    mActiveWidget->setPosition(position);

    mSignal->initialUpdate();

    qDebug("Cell updated: %s = %s", key.toStdString().c_str(), value.toStdString().c_str());
}

void MainWindow::setPropertiesTable(VisuWidget* widget)
{
    QMap<QString, QString> properties = widget->getProperties();
    mActiveWidget = widget;

    disconnect(mPropertiesTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
    mPropertiesTable->clearContents();
    mPropertiesTable->setRowCount(properties.size());
    mPropertiesTable->setColumnCount(2);
    mPropertiesTable->setHorizontalHeaderLabels(QStringList{"Property", "Value"});

    int row = 0;
    for (auto i = properties.begin(); i != properties.end(); ++i)
    {
        //mPropertiesTable->setCellWidget(row, 0, new QLabel(i.key()) );
        //mPropertiesTable->setCellWidget(row, 1, new QLineEdit(i.value()) );

        QString key = i.key();
        QString value = i.value();
        mPropertiesTable->setItem(row, 0, new QTableWidgetItem(key));
        mPropertiesTable->setItem(row, 1, new QTableWidgetItem(value));

        ++row;
    }

    connect(mPropertiesTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
}

VisuSignal* MainWindow::getSignal()
{
    return mSignal;
}

MainWindow::~MainWindow()
{
    delete ui;
}
