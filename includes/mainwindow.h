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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setupToolbarWidgets(QWidget* toolbar);
    VisuSignal* getSignal();
    void setActiveWidget(VisuWidget* widget);
    bool dragOriginIsToolbar(QString originObjectName);
    void keyPressEvent( QKeyEvent *event );
    VisuConfiguration* getConfiguration()
    {
        return mConfiguration;
    }

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
    void setupLayouts();
    void updateMenuSignalList();
    QString mapToString(QMap<QString, QString> properties, int tabs = 0);
    void loadConfigurationFromFile(QString configPath);
    QString configurationToXML();

    static const QString INITIAL_EDITOR_CONFIG;

private slots:
    void openConfiguration();
    void saveConfiguration();
    void saveAsConfiguration();
    void openSignalsEditor();
    void openConfigurationEditor();
    void updateConfig();
};

#endif // MAINWINDOW_H
