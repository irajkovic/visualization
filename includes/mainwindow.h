#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidget>
#include <QMainWindow>
#include "visusignal.h"
#include "wysiwyg/draggablewidget.h"

class Stage;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString xmlPath, QWidget *parent = 0);
    void setupToolbarWidgets(QWidget* toolbar);
    VisuSignal* getSignal();
    void setPropertiesTable(VisuWidget* widget);
    ~MainWindow();

public slots:
    void cellUpdated(int row, int col);

private:
    Ui::MainWindow *ui;
    VisuSignal* mSignal;

    Stage* mStage;
    QTableWidget* mPropertiesTable;
    VisuWidget* mActiveWidget;

    void setupMenu();
    void mapToString(QMap<QString, QString> properties);

private slots:
    void saveConfiguration(bool checked = false);
};

#endif // MAINWINDOW_H
