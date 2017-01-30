#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "visuapplication.h"
#include "instanalog.h"
#include "instdigital.h"
#include "instlinear.h"
#include "insttimeplot.h"
#include "instled.h"
#include "instxyplot.h"
#include "ctrlbutton.h"
#include "visuconfigloader.h"
#include "wysiwyg/visuwidgetfactory.h"
#include <QXmlStreamReader>
#include <QTableWidget>
#include "wysiwyg/stage.h"
#include <QLabel>
#include <QObject>
#include <QColorDialog>

#include <QLineEdit>
#include <QTableWidgetItem>
#include <QtGui>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include "visumisc.h"
#include "wysiwyg/editconfiguration.h"

const QString MainWindow::INITIAL_EDITOR_CONFIG = "system/default.xml";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupMenu();
    setupLayouts();

    loadConfigurationFromFile(INITIAL_EDITOR_CONFIG);
    setupToolbarWidgets(mToolbar);

    setWindowTitle(tr("Configuration Editor"));
    setWindowState(Qt::WindowMaximized);

    show();
}

void MainWindow::setupMenu()
{
    QMenu* fileMenu = ui->menuBar->addMenu(tr("&File"));

    QAction* open = new QAction(tr("&Open"), this);
    open->setShortcut(QKeySequence::Open);
    open->setStatusTip(tr("Open existing configuration"));
    fileMenu->addAction(open);
    connect(open, SIGNAL(triggered()), this, SLOT(openConfiguration()));

    mSave = new QAction(tr("&Save"), this);
    mSave->setDisabled(true);
    mSave->setShortcut(QKeySequence::Save);
    mSave->setStatusTip(tr("Save configuration"));
    fileMenu->addAction(mSave);
    connect(mSave, SIGNAL(triggered()), this, SLOT(saveConfiguration()));

    QAction* saveAs = new QAction(tr("Save &as"), this);
    saveAs->setShortcut(QKeySequence::SaveAs);
    saveAs->setStatusTip(tr("Save as new configuration"));
    fileMenu->addAction(saveAs);
    connect(saveAs, SIGNAL(triggered()), this, SLOT(saveAsConfiguration()));

    QMenu* signalsMenu = ui->menuBar->addMenu(tr("&Signals"));

    QAction* newsig = new QAction(tr("&New"), this);
    newsig->setData(QVariant(-1));
    newsig->setStatusTip(tr("Add new signal"));
    signalsMenu->addAction(newsig);
    connect(newsig, SIGNAL(triggered()), this, SLOT(openSignalsEditor()));

    mSignalsListMenu = signalsMenu->addMenu(tr("&List"));

    QMenu* configMenu = ui->menuBar->addMenu(tr("&Configuration"));

    QAction* configRun = new QAction(tr("&Run"), this);
    configRun->setStatusTip(tr("Run current configuration"));
    configMenu->addAction(configRun);
    connect(configRun, SIGNAL(triggered()), this, SLOT(runConfiguration()));

    QAction* configParams = new QAction(tr("&Parameters"), this);
    configParams->setStatusTip(tr("Edit configuration parameters"));
    configMenu->addAction(configParams);
    connect(configParams, SIGNAL(triggered()), this, SLOT(openConfigurationEditor()));

    QMenu* imagesMenu = ui->menuBar->addMenu(tr("&Images"));

    QAction* imageAdd = new QAction(tr("&Add"), this);
    imageAdd->setStatusTip(tr("Run current configuration"));
    imagesMenu->addAction(imageAdd);
    connect(imageAdd, SIGNAL(triggered()), this, SLOT(openImageAdder()));

}

void MainWindow::setupLayouts()
{
    QWidget* window = new QWidget();
    QVBoxLayout* windowLayout = new QVBoxLayout();
    window->setLayout(windowLayout);
    setCentralWidget(window);

    mToolbar = new QWidget(window);
    mToolbar->setObjectName("toolbar");
    mToolbar->setMinimumHeight(170);
    mToolbar->setStyleSheet("border: 1px solid black;");
    windowLayout->addWidget(mToolbar);

    QWidget* workArea = new QWidget(window);
    windowLayout->addWidget(workArea);

    QHBoxLayout* workAreaLayout = new QHBoxLayout();
    workArea->setLayout(workAreaLayout);

    workAreaLayout->addStretch();

    mStage = new Stage(this, workArea);
    mStage->setObjectName("stage");
    workAreaLayout->addWidget(mStage);

    workAreaLayout->addStretch();

    mPropertiesTable = new QTableWidget(workArea);
    mPropertiesTable->setMaximumWidth(300);
    mPropertiesTable->verticalHeader()->hide();
    mPropertiesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    workAreaLayout->addWidget(mPropertiesTable);
}

void MainWindow::unloadConfiguration()
{
    delete mConfiguration;

}

void MainWindow::loadConfigurationFromFile(const QString& configPath)
{
    unloadConfiguration();
    mConfiguration = new VisuConfiguration();

    try
    {
        QString xml = VisuConfigLoader::loadXMLFromFile(configPath);
        mConfiguration->fromXML(mStage, QString(xml));

        updateConfig();

        // connect widgets
        for (VisuWidget* widget : mConfiguration->getWidgets())
        {
            connect(widget, SIGNAL(widgetActivated(VisuWidget*)), mStage, SLOT(activateWidget(VisuWidget*)));
        }

        updateMenuSignalList();
    }
    catch(ConfigLoadException e)
    {
        QMessageBox::warning(
                    this,
                    "Error",
                    e.what());
    }
    catch(...)
    {
        QMessageBox::warning(
                    this,
                    "Error",
                    "Unknown exception!");
    }
}

void MainWindow::updateConfig()
{
    mStage->setMaximumSize(mConfiguration->getWidth(), mConfiguration->getHeight());
    mStage->setMinimumSize(mConfiguration->getWidth(), mConfiguration->getHeight());
    VisuMisc::setBackgroundColor(mStage, mConfiguration->getBackgroundColor());
}

void MainWindow::updateMenuSignalList()
{
    mSignalsListMenu->clear();
    auto configSignals = mConfiguration->getSignals();
    if (configSignals.size() > 0)
    {
        for (VisuSignal* sig : configSignals)
        {
            if (sig != nullptr)
            {
                QMenu* tmpSig = mSignalsListMenu->addMenu(sig->getName());

                QAction* edit = new QAction(tr("&Edit"), this);
                edit->setData(QVariant(sig->getId()));
                tmpSig->addAction(edit);
                connect(edit, SIGNAL(triggered()), this, SLOT(openSignalsEditor()));

                QAction* del = new QAction(tr("&Delete"), this);
                del->setData(QVariant(sig->getId()));
                tmpSig->addAction(del);
                connect(del, SIGNAL(triggered()), this, SLOT(deleteSignal()));
            }
        }
    }
}

void MainWindow::setupToolbarWidgets(QPointer<QWidget> toolbar)
{
    QHBoxLayout *layout = new QHBoxLayout;
    toolbar->setLayout(layout);

    VisuSignal* toolbarSignal = mConfiguration->getSignal(0);

    layout->addWidget(VisuWidgetFactory::createWidget(this, InstAnalog::TAG_NAME, toolbarSignal));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstLinear::TAG_NAME, toolbarSignal));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstTimePlot::TAG_NAME, toolbarSignal));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstDigital::TAG_NAME, toolbarSignal));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstLED::TAG_NAME, toolbarSignal));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstXYPlot::TAG_NAME, toolbarSignal));

    layout->addWidget(VisuWidgetFactory::createWidget(this, CtrlButton::TAG_NAME));

    toolbarSignal->initializeInstruments();
}

void MainWindow::openImageAdder()
{
    QString imagePath = QFileDialog::getOpenFileName(this,
                                                      tr("Open image"),
                                                      ".",
                                                      "Image files (*.png *.jpg *.jpeg *.bmp)");
    if (!imagePath.isNull())
    {

        QFile file(imagePath);

        if (file.open(QIODevice::ReadOnly))
        {
            QMap<QString, QString> properties = VisuConfigLoader::getMapFromFile("system/IMAGE.xml", VisuConfiguration::TAG_WIDGET);
            QByteArray imgData = file.readAll();

            properties["image"] = QString(imgData.toBase64());
            properties["format"] = QFileInfo(imagePath).suffix();

            QImage tmpImage;
            tmpImage.loadFromData(imgData, properties["format"].toStdString().c_str());

            if ((tmpImage.width() <= mStage->width()) & (tmpImage.height() <= mStage->height()) )
            {
                properties["width"] = QString("%1").arg(tmpImage.width());
                properties["height"] = QString("%1").arg(tmpImage.height());

                StaticImage* image = new StaticImage(mStage, properties);
                mConfiguration->addWidget(image);
                mStage->update();
            }
            else
            {
                QMessageBox::information(
                            this,
                            tr("Info"),
                            tr("Image was too large for the stage.\n"
                               "Please try with smaller image, or increase stage resolution"));
            }
        }
    }
}

void MainWindow::openConfigurationEditor()
{
    EditConfiguration* window = new EditConfiguration(this, mConfiguration);
    connect(window, SIGNAL(configParamsUpdated()), this, SLOT(updateConfig()));
}

void MainWindow::runConfiguration()
{
    QString xml = mConfiguration->toXML();
    QString configFilePath = VisuMisc::saveToFile(mTmpConfigFile, xml);
    QString me = QCoreApplication::applicationFilePath();

    QStringList args = {configFilePath};
    QProcess *process = new QProcess(this);

    process->start(me, args);
}

void MainWindow::openSignalsEditor()
{
    if (editSignalWindow != nullptr)
    {
        disconnect(editSignalWindow, SIGNAL(signalAdded(VQPointer<VisuSignal>)), this, SLOT(addSignal(QPointer<VisuSignal>, bool)));
    }

    QAction* s = static_cast<QAction*>(sender());
    int signalId = s->data().toInt();
    VisuSignal* signal = nullptr;
    if (signalId >= 0)
    {
        signal = mConfiguration->getSignal(signalId);
    }
    editSignalWindow = new EditSignal(this, signal);
    connect(editSignalWindow, SIGNAL(signalAdded(QPointer<VisuSignal>,bool)), this, SLOT(addSignal(QPointer<VisuSignal>, bool)));
}

void MainWindow::openConfiguration()
{
    QString configPath = QFileDialog::getOpenFileName(this,
                                                      tr("Open configuration"),
                                                      ".",
                                                      "Configuration files (*.xml)");
    if (!configPath.isNull())
    {
        loadConfigurationFromFile(configPath);
        mConfigPath = configPath;
        mSave->setDisabled(false);
    }
}

void MainWindow::saveAsConfiguration()
{
    QString configPath = QFileDialog::getSaveFileName(this,
                                                      tr("Save configuration"),
                                                      ".",
                                                      "Configuration files (*.xml)");
    if (!configPath.isNull())
    {
        QFile file( configPath );
        QString xml = mConfiguration->toXML();
        VisuMisc::saveToFile(file, xml);
        mConfigPath = configPath;
        mSave->setDisabled(false);
    }
}

void MainWindow::saveConfiguration()
{
    if (!mConfigPath.isNull())
    {
        QFile file( mConfigPath );
        QString xml = mConfiguration->toXML();
        VisuMisc::saveToFile(file, xml);
    }
}

void MainWindow::keyPressEvent( QKeyEvent *event )
{
    if (mActiveWidget != nullptr)
    {
        if (event->matches(QKeySequence::Delete))
        {
            mConfiguration->deleteWidget(mActiveWidget);
            mPropertiesTable->clearContents();
            mActiveWidget = nullptr;
        }
    }
}

bool MainWindow::dragOriginIsToolbar(QWidget* widget)
{
    return widget->parent() == mToolbar;
}

void MainWindow::addSignal(QPointer<VisuSignal> signal, bool isNewSignal)
{
    if (isNewSignal)
    {
        mConfiguration->addSignal(signal);
    }

    updateMenuSignalList();
}

void MainWindow::deleteSignal()
{
    QAction* s = static_cast<QAction*>(sender());
    int signalId = s->data().toInt();
    if (signalId >= 0)
    {
        mConfiguration->deleteSignal(signalId);
        updateMenuSignalList();
    }
}

void MainWindow::cellUpdated(int row, int col)
{
    (void)col;

    QString key = mPropertiesTable->item(row,0)->text();
    QString value = VisuMisc::getValueString(row, key, mPropertiesTable);

    QMap<QString, QString> properties = mActiveWidget->getProperties();
    QPoint position = mActiveWidget->pos();

    if (key == VisuMisc::PROP_X)
    {
        position.setX(value.toInt());
    }
    else if (key == VisuMisc::PROP_Y)
    {
        position.setY(value.toInt());
    }
    else if (key == VisuMisc::PROP_SIGNAL_ID)
    {
        // find old signal and detach instrument
        VisuInstrument* inst = qobject_cast<VisuInstrument*>(mActiveWidget);
        mConfiguration->detachInstrumentFromSignal(inst);
    }

    properties[key] = value;
    mActiveWidget->loadProperties(properties);
    mActiveWidget->setPosition(position);

    // refresh instrument
    VisuInstrument* inst = qobject_cast<VisuInstrument*>(mActiveWidget);
    if (inst != nullptr)
    {
        VisuSignal* signal = mConfiguration->getSignal(inst->getSignalId());
        if (key == VisuMisc::PROP_SIGNAL_ID)
        {
            mConfiguration->attachInstrumentToSignal(inst);
        }
        signal->initializeInstruments();
    }

    // refresh controls
    CtrlButton* btn = qobject_cast<CtrlButton*>(mActiveWidget);
    if (btn != nullptr)
    {
        btn->redraw();
    }
}

void MainWindow::setActiveWidget(QPointer<VisuWidget> widget)
{
    if (mActiveWidget != nullptr)
    {
        // remove selected style. TODO :: refactor to work with other classes
        mActiveWidget->setActive(false);
    }

    QMap<QString, QString> properties = widget->getProperties();
    mActiveWidget = widget;

    disconnect(mPropertiesTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
    VisuMisc::updateTable(mPropertiesTable,
                          properties,
                          &(mConfiguration->getSignals()),
                          this,
                          SLOT(propertyChange()));
    connect(mPropertiesTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));

    mActiveWidget->setActive(true);
}

void MainWindow::propertyChange(int parameter)
{
    int row = VisuMisc::updateWidgetProperty(sender(), this);
    cellUpdated(row, 1);
}

QPointer<VisuSignal> MainWindow::getSignal()
{
    return mConfiguration->getSignal(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPointer<VisuConfiguration> MainWindow::getConfiguration()
{
    return mConfiguration;
}

QPointer<VisuWidget> MainWindow::getActiveWidget()
{
    return mActiveWidget;
}
