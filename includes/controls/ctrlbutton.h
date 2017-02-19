#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QUdpSocket>
#include <QHBoxLayout>
#include "visuhelper.h"
#include "visucontrol.h"

class CtrlButton : public VisuControl
{
    public:
        explicit CtrlButton(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuControl(parent, properties)
        {

            loadProperties(properties);
            setup(parent);
        }

        static const QString TAG_NAME;

        virtual void loadProperties(QMap<QString, QString> properties);
        virtual void refresh(const QString& key);
        void setup(QWidget* parent);

private:

    Q_OBJECT

    QString cActionMessage;
    QString cCss;
    QPushButton* mButton;
    QHBoxLayout* mLayout;

    void setupButton(QWidget* parent);

private slots:
    void sendCommand();

};

#endif // BUTTON_H
