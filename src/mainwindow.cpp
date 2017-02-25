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
#include <QScrollArea>
#include "visumisc.h"
#include "wysiwyg/editconfiguration.h"
#include "wysiwyg/visupropertieshelper.h"

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
    showMaximized();
    updateConfig();

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
    QAction* newsig = new QAction(tr("&Add"), this);
    newsig->setData(QVariant(-1));
    newsig->setStatusTip(tr("Add new signal"));
    signalsMenu->addAction(newsig);
    connect(newsig, SIGNAL(triggered()), this, SLOT(openSignalsEditor()));
    mSignalsListMenu = signalsMenu->addMenu(tr("&List"));

    QMenu* instrumentsMenu = ui->menuBar->addMenu(tr("&Widgets"));
    QAction* imageAdd = new QAction(tr("&Add image"), this);
    imageAdd->setStatusTip(tr("Add image widget"));
    instrumentsMenu->addAction(imageAdd);
    connect(imageAdd, SIGNAL(triggered()), this, SLOT(openImageAdder()));
    mWidgetsListMenu = instrumentsMenu->addMenu(tr("&List"));

    QMenu* configMenu = ui->menuBar->addMenu(tr("&Configuration"));

    QAction* configRun = new QAction(tr("&Run"), this);
    configRun->setStatusTip(tr("Run current configuration"));
    configMenu->addAction(configRun);
    connect(configRun, SIGNAL(triggered()), this, SLOT(runConfiguration()));

    QAction* configParams = new QAction(tr("&Parameters"), this);
    configParams->setStatusTip(tr("Edit configuration parameters"));
    configMenu->addAction(configParams);
    connect(configParams, SIGNAL(triggered()), this, SLOT(openConfigurationEditor()));

    QAction* screenshot = new QAction(tr("Take snapshot"), this);
    screenshot->setStatusTip(tr("Save configuration to image"));
    configMenu->addAction(screenshot);
    connect(screenshot, SIGNAL(triggered()), this, SLOT(saveToImage()));


}

void MainWindow::setupLayouts()
{
    mWindow = new QWidget();
    QVBoxLayout* windowLayout = new QVBoxLayout();
    mWindow->setLayout(windowLayout);
    setCentralWidget(mWindow);

    mToolbar = new QWidget(mWindow);
    mToolbar->setObjectName("toolbar");
    mToolbar->setMinimumHeight(LAYOUT_TOOLBAR_HEIGHT);
    mToolbar->setStyleSheet("border: 1px solid black;");
    windowLayout->addWidget(mToolbar);

    QWidget* workArea = new QWidget(mWindow);
    windowLayout->addWidget(workArea);

    QHBoxLayout* workAreaLayout = new QHBoxLayout();
    workArea->setLayout(workAreaLayout);

    workAreaLayout->addStretch();

    mStage = new Stage(this, workArea);
    mStage->setObjectName("stage");

    mScrollArea = new QScrollArea(mWindow);
    workAreaLayout->addWidget(mScrollArea);
    mScrollArea->setWidget(mStage);

    workAreaLayout->addStretch();

    mPropertiesTable = new QTableWidget(workArea);
    mPropertiesTable->setMaximumWidth(LAYOUT_PROPERTIES_WIDTH);
    mPropertiesTable->setMinimumWidth(LAYOUT_PROPERTIES_WIDTH);
    mPropertiesTable->verticalHeader()->hide();
    mPropertiesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    workAreaLayout->addWidget(mPropertiesTable);
}

void MainWindow::reloadConfiguration()
{
    QString xml = mConfiguration->toXML();
    QString configFilePath = VisuMisc::saveToFile(mTmpConfigFile, xml);
    loadConfigurationFromFile(configFilePath);
}

void MainWindow::loadConfigurationFromFile(const QString& configPath)
{
    mConfiguration = VisuConfiguration::getClean();
    try
    {
        QString xml = VisuConfigLoader::loadXMLFromFile(configPath);
        mConfiguration->fromXML(mStage, QString(xml));
        mConfigChanged = false;
        updateConfig();

        // connect widgets
        for (VisuWidget* widget : mConfiguration->getWidgets())
        {
            connect(widget, SIGNAL(widgetActivated(VisuWidget*)), mStage, SLOT(activateWidget(VisuWidget*)));
        }

        updateMenuSignalList();
        updateMenuWidgetsList();
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
    QSize configSize = mConfiguration->getSize();
    QSize windowSize = mWindowSize;
    QSize areaSize = mScrollArea->size();

    mStage->setMinimumSize(configSize);
    mStage->setMaximumSize(configSize);

    if (!windowSize.isValid())
    {
        return;
    }

    int sliderThickness = qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    int sliderVerticalMargin = (areaSize.width() < configSize.width() ? 0 : sliderThickness) + LAYOUT_QSCROLLAREA_MARGIN;
    int sliderHorizontalMargin = (areaSize.height() < configSize.height() ? 0 : sliderThickness) + LAYOUT_QSCROLLAREA_MARGIN;

    int maxAvailableWidth = windowSize.width() - LAYOUT_PROPERTIES_WIDTH - LAYOUT_MARGIN;
    int neededWidth = configSize.width() + sliderVerticalMargin;
    if (maxAvailableWidth > neededWidth)
    {
        mScrollArea->setMinimumWidth(neededWidth);
    }
    else
    {
        mScrollArea->setMinimumWidth(maxAvailableWidth);
        mScrollArea->setMaximumWidth(maxAvailableWidth);
    }

    int verticalOcupiedSpace = this->frameGeometry().height() - this->geometry().height();
    verticalOcupiedSpace += LAYOUT_TOOLBAR_HEIGHT + LAYOUT_MARGIN * 2;
    int maxAvailableHeight = windowSize.height() - verticalOcupiedSpace;
    int neededHeight = configSize.height() + sliderHorizontalMargin;
    if (maxAvailableHeight > neededHeight )
    {
       mScrollArea->setMinimumHeight(neededHeight);
    }
    else
    {
       mScrollArea->setMinimumHeight(maxAvailableHeight);
       mScrollArea->setMinimumHeight(maxAvailableHeight);
    }

    VisuMisc::setBackgroundColor(mStage, mConfiguration->getBackgroundColor());
}

void MainWindow::updateMenuWidgetsList()
{
    mWidgetsListMenu->clear();
    auto widgets = mConfiguration->getWidgets();
    if (widgets.size() > 0)
    {
        for (VisuWidget* widget : widgets)
        {
            if (widget != nullptr)
            {
                QString menuItemText = QString("%1 (%2)")
                        .arg(widget->getType())
                        .arg(widget->getName());

                QMenu* widgetMenuItem = mWidgetsListMenu->addMenu(menuItemText);

                QAction* select = new QAction(tr("Select"), this);
                select->setData(QVariant(widget->getId()));
                widgetMenuItem->addAction(select);
                connect(select, SIGNAL(triggered()), this, SLOT(activateWidgetFromMenu()));

                QAction* moveUp = new QAction(tr("Move up"), this);
                moveUp->setData(QVariant(widget->getId()));
                widgetMenuItem->addAction(moveUp);
                connect(moveUp, SIGNAL(triggered()), this, SLOT(moveWidgetUp()));

                QAction* moveDown = new QAction(tr("Move down"), this);
                moveDown->setData(QVariant(widget->getId()));
                widgetMenuItem->addAction(moveDown);
                connect(moveDown, SIGNAL(triggered()), this, SLOT(moveWidgetDown()));
            }
        }
    }
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

    layout->addWidget(VisuWidgetFactory::createWidget(this, InstAnalog::TAG_NAME));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstLinear::TAG_NAME));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstTimePlot::TAG_NAME));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstDigital::TAG_NAME));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstLED::TAG_NAME));
    layout->addWidget(VisuWidgetFactory::createWidget(this, InstXYPlot::TAG_NAME));
    layout->addWidget(VisuWidgetFactory::createWidget(this, CtrlButton::TAG_NAME));

    mConfiguration->initializeInstruments();
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
            QMap<QString, QString> properties = VisuConfigLoader::getMapFromFile(StaticImage::TAG_NAME, VisuWidget::TAG_NAME);
            QByteArray imgData = file.readAll();

            properties[StaticImage::KEY_IMAGE] = QString(imgData.toBase64());
            properties[StaticImage::KEY_FORMAT] = QFileInfo(imagePath).suffix();

            QImage tmpImage;
            tmpImage.loadFromData(imgData, properties[StaticImage::KEY_FORMAT].toStdString().c_str());

            if ((tmpImage.width() <= mStage->width()) & (tmpImage.height() <= mStage->height()) )
            {
                properties[VisuWidget::KEY_WIDTH] = QString("%1").arg(tmpImage.width());
                properties[VisuWidget::KEY_HEIGHT] = QString("%1").arg(tmpImage.height());
                properties[VisuWidget::KEY_NAME] = QFileInfo(imagePath).fileName();

                QMap<QString, VisuPropertyMeta> metaProperties =
                        VisuConfigLoader::getMetaMapFromFile(StaticImage::TAG_NAME, VisuWidget::TAG_NAME);

                StaticImage* image = new StaticImage(mStage, properties, metaProperties);
                setActiveWidget(image);
                mConfiguration->addWidget(image);
                mConfigChanged = true;
                updateMenuWidgetsList();
            }
            else
            {
                QMessageBox::information(
                            this,
                            tr("Info"),
                            tr("Image was too large for the stage.\n"
                               "Please try with smaller image, or increase stage resolution to %1x%2")
                               .arg(tmpImage.width()).arg(tmpImage.height()));
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
    if (confirmLoseChanges())
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
        mConfigChanged = false;
    }
}

void MainWindow::saveToImage()
{
    QString configPath = QFileDialog::getSaveFileName(this,
                                                      tr("Save configuration"),
                                                      ".",
                                                      "Image file (*.png)");
    if (!configPath.isNull())
    {
        QPixmap image = mStage->grab();
        image.save(configPath);
    }
}

void MainWindow::saveConfiguration()
{
    if (!mConfigPath.isNull())
    {
        QFile file( mConfigPath );
        QString xml = mConfiguration->toXML();
        VisuMisc::saveToFile(file, xml);
        mConfigChanged = false;
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

void MainWindow::refreshEditorGui(QString key)
{
    mConfigChanged = true;
    if (key == VisuWidget::KEY_NAME)
    {
        updateMenuWidgetsList();    // widget renamed, update list
    }
}

void MainWindow::cellUpdated(int row, int col)
{
    (void)col;

    QString key = mPropertiesTable->item(row,0)->text();
    QString value = VisuPropertiesHelper::getValueString(mPropertiesTable, row);

    if (mActiveWidget->updateProperties(key, value))
    {
        setActiveWidget(mActiveWidget);
    }
    refreshEditorGui(key);
}

VisuWidget* MainWindow::actionDataToWidget(QAction* action)
{
    VisuWidget* widget = mConfiguration->getWidget(action->data().toInt());
    return widget;
}

void MainWindow::activateWidgetFromMenu()
{
    QAction* action = static_cast<QAction*>(sender());
    VisuWidget* widget = actionDataToWidget(action);
    setActiveWidget(widget);
}

void MainWindow::moveWidgetUp()
{
    QAction* action = static_cast<QAction*>(sender());
    mConfiguration->moveWidgetUp(action->data().toInt());
    reloadConfiguration();
}

void MainWindow::moveWidgetDown()
{
    QAction* action = static_cast<QAction*>(sender());
    mConfiguration->moveWidgetDown(action->data().toInt());
    reloadConfiguration();
}


void MainWindow::markActiveInstrumentMenuItem(QPointer<VisuWidget> oldItem, QPointer<VisuWidget> newItem)
{
    for(QAction* action : mWidgetsListMenu->actions())
    {
        VisuWidget* menuWidget = actionDataToWidget(action->menu()->actions()[0]);
        action->setCheckable(true);

        if (menuWidget == oldItem)
        {
            action->setChecked(false);
        }

        if (menuWidget == newItem)
        {
            action->setChecked(true);
        }
    }
}

void MainWindow::setActiveWidget(QPointer<VisuWidget> widget)
{
    if (mActiveWidget != nullptr)
    {
        // remove selected style. TODO :: refactor to work with other classes
        mActiveWidget->setActive(false);
    }
    markActiveInstrumentMenuItem(mActiveWidget, widget);
    mActiveWidget = widget;
    mActiveWidget->setActive(true);

    QMap<QString, QString> properties = widget->getProperties();
    QMap<QString, VisuPropertyMeta> metaProperties = mActiveWidget->getPropertiesMeta();

    disconnect(mPropertiesTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
    VisuPropertiesHelper::updateTable(mPropertiesTable,
                          properties,
                          metaProperties,
                          std::make_pair(this, SLOT(propertyChange()))
                          );
    connect(mPropertiesTable, SIGNAL(cellChanged(int,int)), this, SLOT(cellUpdated(int,int)));
}

void MainWindow::propertyChange(int parameter)
{
    int row = VisuPropertiesHelper::updateWidgetProperty(sender(), this);
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

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);

    mWindowSize = size();
}

void MainWindow::setChanged()
{
    mConfigChanged = true;
}

bool MainWindow::confirmLoseChanges()
{
    QMessageBox::StandardButton response = QMessageBox::question( this, "Warning",
                                                                tr("There are unsaved changes that will be lost. "
                                                                   "Are you sure you want to proceed?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);

    return response == QMessageBox::Yes;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (mConfigChanged)
    {
        bool response = confirmLoseChanges();
        if (response)
        {
            event->accept();
        } else
        {
            event->ignore();
        }
    }
}
