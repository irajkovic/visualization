#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "visuapplication.h"
#include "instanalog.h"
#include "instdigital.h"
#include "instlinear.h"
#include "insttimeplot.h"
#include "instled.h"
#include "visuconfigloader.h"
#include "wysiwyg/draggablewidget.h"
#include <QXmlStreamReader>

MainWindow::MainWindow(QString xmlPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupToolbarWidgets();

    VisuApplication *application = new VisuApplication(xmlPath);

    ui->horizontalLayout->addWidget(application);
    setWindowState(Qt::WindowMaximized);
    show();
}

#define ADD_INSTRUMENT_TO_LAYOUT(CLASS, FILE, LAYOUT)\
    DraggableWidget* draggable##CLASS = new DraggableWidget(this); \
    CLASS* FILE = new CLASS(draggable##CLASS, VisuConfigLoader::getTagFromFile("system/"#FILE".xml", "instrument")); \
    draggable##CLASS->setWidget(FILE); \
    signal->connectInstrument(FILE); \
    LAYOUT->addWidget(draggable##CLASS);


void MainWindow::setupToolbarWidgets()
{
    QHBoxLayout *layout = new QHBoxLayout;
    ui->toolbox->setLayout(layout);

    VisuSignal* signal = new VisuSignal(VisuConfigLoader::getTagFromFile("system/signal.xml", "signal"));

    ADD_INSTRUMENT_TO_LAYOUT(InstAnalog, analog, layout);
    ADD_INSTRUMENT_TO_LAYOUT(InstLinear, linear, layout);
    ADD_INSTRUMENT_TO_LAYOUT(InstTimePlot, timeplot, layout);
    ADD_INSTRUMENT_TO_LAYOUT(InstDigital, digital, layout);
    ADD_INSTRUMENT_TO_LAYOUT(InstLED, led, layout);

    signal->initialUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}
