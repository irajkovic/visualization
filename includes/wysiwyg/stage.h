#ifndef STAGE_H
#define STAGE_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "mainwindow.h"

class Stage : public QWidget
{
    Q_OBJECT

public:
    Stage(MainWindow* window, QWidget* parent = 0) : QWidget(parent)
    {
        setAcceptDrops(true);
        mMainWindow = window;
    }

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    void activateWidget(VisuWidget*);

private:
    MainWindow* mMainWindow;
};

#endif // STAGE_H
