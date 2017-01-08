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

void MainWindow::setPropertiesTable(QMap<QString, QString> properties)
{
    mPropertiesTable->clearContents();
    mPropertiesTable->setRowCount(properties.size());
    mPropertiesTable->setColumnCount(2);
    mPropertiesTable->setHorizontalHeaderLabels(QStringList{"Property", "Value"});

    int row = 0;
    for (auto i = properties.begin(); i != properties.end(); ++i)
    {
        mPropertiesTable->setCellWidget(row, 0, new QLabel(i.key()) );
        mPropertiesTable->setCellWidget(row, 1, new QLineEdit(i.value()) );
        ++row;
    }
}

VisuSignal* MainWindow::getSignal()
{
    return mSignal;
}

MainWindow::~MainWindow()
{
    delete ui;
}
