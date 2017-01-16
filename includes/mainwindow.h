#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QTableWidget>
#include <QMainWindow>
#include <QPointer>
#include <QTemporaryFile>
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
    bool dragOriginIsToolbar(QWidget *widget);
    void keyPressEvent( QKeyEvent *event );
    QPointer<VisuConfiguration> getConfiguration();
    QPointer<VisuWidget> getActiveWidget();

    ~MainWindow();

public slots:

    void cellUpdated(int row, int col);
    void addSignal(QPointer<VisuSignal> signal, bool isNewSignal);
    void deleteSignal();
    void propertyChange(int parameter = 0);

private:
    Ui::MainWindow *ui;

    QPointer<QWidget> mToolbar;
    QPointer<Stage> mStage;
    QPointer<QTableWidget> mPropertiesTable;
    QPointer<VisuWidget> mActiveWidget;
    QPointer<VisuConfiguration> mConfiguration;
    QPointer<EditSignal> editSignalWindow;
    QPointer<QMenu> mSignalsListMenu;
    QTemporaryFile mTmpConfigFile;
    QString mConfigPath;
    QPointer<QAction> mSave;

    void setupMenu();
    void setupLayouts();
    void updateMenuSignalList();
    void loadConfigurationFromFile(const QString& configPath);
    QString configurationToXML();

    static const QString INITIAL_EDITOR_CONFIG;

private slots:
    void openConfiguration();
    void saveConfiguration();
    void saveAsConfiguration();
    void openSignalsEditor();
    void openConfigurationEditor();
    void updateConfig();
    void runConfiguration();
};

#endif // MAINWINDOW_H
