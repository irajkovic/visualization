#ifndef VISUAPPLICATION_H
#define VISUAPPLICATION_H

#include <QWidget>
#include <QByteArray>
#include <QString>

#include "visuconfiguration.h"
#include "visuserver.h"


class VisuApplication : public QWidget
{
    private:
        VisuConfiguration* configuration;
        VisuServer *server;
        void setBackgroundColor();
        void setupWindow();
        void loadConfiguration(QString path);

    public:
        VisuApplication(QString path);
        void run();

};

#endif // VISUAPPLICATION_H
