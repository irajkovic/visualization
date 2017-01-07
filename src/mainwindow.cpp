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

#include "wysiwyg/stage.h"

MainWindow::MainWindow(QString xmlPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupToolbarWidgets();

    //VisuApplication *application = new VisuApplication(xmlPath);
    Stage* stage = new Stage(this);
    stage->setStyleSheet("background-color: gray;");

    ui->horizontalLayout->addWidget(stage);
    setWindowState(Qt::WindowMaximized);
    show();
}

void MainWindow::setupToolbarWidgets()
{
    QHBoxLayout *layout = new QHBoxLayout;
    ui->toolbox->setLayout(layout);

    mSignal = new VisuSignal(VisuConfigLoader::getTagFromFile("system/signal.xml", "signal"));

    ADD_INSTRUMENT_TO_LAYOUT(this, InstAnalog, analog, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstLinear, linear, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstTimePlot, timeplot, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstDigital, digital, mSignal, layout);
    ADD_INSTRUMENT_TO_LAYOUT(this, InstLED, led, mSignal, layout);

    mSignal->initialUpdate();
}

VisuSignal* MainWindow::getSignal()
{
    return mSignal;
}

MainWindow::~MainWindow()
{
    delete ui;
}
