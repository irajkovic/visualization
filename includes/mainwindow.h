#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QTableWidget>
#include <QMainWindow>
#include <QPointer>
#include <QTemporaryFile>
#include <QScrollArea>
#include "visusignal.h"
#include "visuconfiguration.h"
#include "wysiwyg/editsignal.h"
class Stage;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setupToolbarWidgets(QPointer<QWidget> toolbar);
    QPointer<VisuSignal> getSignal();
    void setActiveWidget(QPointer<VisuWidget> widget);
    void setChanged();
    bool dragOriginIsToolbar(QWidget *widget);
    void keyPressEvent( QKeyEvent *event );
    QPointer<VisuConfiguration> getConfiguration();
    QPointer<VisuWidget> getActiveWidget();
    void updateMenuWidgetsList();
    void closeEvent(QCloseEvent* event);
    ~MainWindow();

public slots:

    void cellUpdated(int row, int col);
    void addSignal(QPointer<VisuSignal> signal, bool isNewSignal);
    void deleteSignal();
    void propertyChange(int parameter = 0);

private:
    Ui::MainWindow *ui;

    QPointer<QWidget> mWindow;
    QPointer<QWidget> mToolbar;
    QPointer<Stage> mStage;
    QPointer<QScrollArea> mScrollArea;
    QPointer<QTableWidget> mPropertiesTable;
    QPointer<VisuWidget> mActiveWidget;
    QPointer<VisuConfiguration> mConfiguration;
    QPointer<EditSignal> editSignalWindow;
    QPointer<QMenu> mSignalsListMenu;
    QPointer<QMenu> mWidgetsListMenu;
    QTemporaryFile mTmpConfigFile;
    QString mConfigPath;
    QPointer<QAction> mSave;
    QSize mWindowSize;
    bool mConfigChanged;
    void resizeEvent(QResizeEvent * event);

    void setupMenu();
    void setupLayouts();
    void updateMenuSignalList();
    void reloadConfiguration();
    void loadConfigurationFromFile(const QString& configPath);
    QString configurationToXML();
    void markActiveInstrumentMenuItem(QPointer<VisuWidget> oldItem, QPointer<VisuWidget> newItem);
    VisuWidget* actionDataToWidget(QAction* action);
    void refreshEditorGui(QString key);
    bool confirmLoseChanges();

    static const QString INITIAL_EDITOR_CONFIG;
    static const int LAYOUT_TOOLBAR_HEIGHT = 170;
    static const int LAYOUT_PROPERTIES_WIDTH = 300;
    static const int LAYOUT_MARGIN = 50;
    static const int LAYOUT_QSCROLLAREA_MARGIN = 5;   // Margin between child widget and QScrollArea

private slots:
    void openConfiguration();
    void saveConfiguration();
    void saveAsConfiguration();
    void saveToImage();
    void openSignalsEditor();
    void openConfigurationEditor();
    void updateConfig();
    void runConfiguration();
    void openImageAdder();
    void activateWidgetFromMenu();
    void moveWidgetUp();
    void moveWidgetDown();
};

#endif // MAINWINDOW_H
