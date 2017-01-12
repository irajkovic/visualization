#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QTableWidget>
#include <QMainWindow>
#include "visusignal.h"
#include "visuconfiguration.h"
#include "wysiwyg/editsignal.h"
class Stage;

namespace Ui {
class MainWindow;
}

enum DRAG_ORIGIN
{
    TOOLBAR = 0,
    STAGE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString xmlPath, QWidget *parent = 0);
    void setupToolbarWidgets(QWidget* toolbar);
    VisuSignal* getSignal();
    void setActiveWidget(VisuWidget* widget);
    bool dragOriginIsToolbar(QString originObjectName);
    VisuWidget* getActiveWidget()
    {
        return mActiveWidget;
    }

    ~MainWindow();

public slots:

    void cellUpdated(int row, int col);
    void addSignal(VisuSignal* signal, bool isNewSignal);
    void deleteSignal();

private:
    Ui::MainWindow *ui;

    QWidget* mToolbar;
    Stage* mStage;
    QTableWidget* mPropertiesTable;
    VisuWidget* mActiveWidget;
    VisuConfiguration* mConfiguration;
    EditSignal* editSignalWindow;
    QMenu* mSignalsListMenu;

    void setupMenu();
    void updateMenuSignalList();
    QString mapToString(QMap<QString, QString> properties, int tabs = 0);
    void loadConfigurationFromFile(QString configPath);
    QString configurationToXML();

private slots:
    void openConfiguration();
    void saveConfiguration();
    void saveAsConfiguration();
    void openSignalsEditor();
    void openConfigurationEditor();
    void updateConfig();
};

#endif // MAINWINDOW_H
