#ifndef VISUAPPLICATION_H
#define VISUAPPLICATION_H

#include <QWidget>
#include <QByteArray>
#include <QString>

#include "configuration.h"
#include "server.h"


class VisuApplication : public QWidget
{
    private:
        VisuConfiguration configuration;
        VisuServer *server;
        QByteArray loadXMLFromFile(QString path);
        void setBackgroundColor();
        void setupWindow();
        void loadConfiguration(QString path);

    public:
        VisuApplication(QString path);
        void run();

};

#endif // VISUAPPLICATION_H
