#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidget>
#include <QMainWindow>
#include "visusignal.h"

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
    void setPropertiesTable(VisuWidget* widget);
    bool dragOriginIsToolbar(QString originObjectName);
    VisuWidget* getActiveWidget()
    {
        return mActiveWidget;
    }

    ~MainWindow();

public slots:
    void cellUpdated(int row, int col);

private:
    Ui::MainWindow *ui;
    VisuSignal* mSignal;

    QWidget* mToolbar;
    Stage* mStage;
    QTableWidget* mPropertiesTable;
    VisuWidget* mActiveWidget;

    void setupMenu();
    void mapToString(QMap<QString, QString> properties);

private slots:
    void saveConfiguration(bool checked = false);
};

#endif // MAINWINDOW_H
