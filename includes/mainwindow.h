#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void setupToolbarWidgets();
    VisuSignal* getSignal();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VisuSignal* mSignal;
};

#endif // MAINWINDOW_H
