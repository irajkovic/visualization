#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidget>
#include <QMainWindow>
#include "visusignal.h"
#include "wysiwyg/draggablewidget.h"

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
    void setPropertiesTable(QMap<QString, QString> properties);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VisuSignal* mSignal;

    QTableWidget* mPropertiesTable;
};

#endif // MAINWINDOW_H
