#ifndef STAGE_H
#define STAGE_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "mainwindow.h"

class Stage : public QWidget
{
public:
    Stage(QWidget* parent) : QWidget(parent)
    {
        setAcceptDrops(true);
        mMainWindow = static_cast<MainWindow*>(parent);
    }

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    MainWindow* mMainWindow;
};

#endif // STAGE_H
