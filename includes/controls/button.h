#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QUdpSocket>
#include <QHBoxLayout>
#include "visuhelper.h"
#include "visuwidget.h"

class Button : public VisuWidget
{
    public:
        explicit Button(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuWidget(parent, properties)
        {

            loadProperties(properties);

            show();

            mTagName = TAG_NAME;
            mAddress = QHostAddress(cActionIp);
            mLayout = new QHBoxLayout();
            mLayout->setContentsMargins(0,0,0,0);
            setLayout(mLayout);

            setupButton(parent);
        }

        static const QString TAG_NAME;

        virtual void loadProperties(QMap<QString, QString> properties);
        void redraw();

private:

    Q_OBJECT

    QString cActionIp;
    quint16 cActionPort;
    QString cActionMessage;
    QString cCss;

    QPushButton* mButton;
    QHostAddress mAddress;
    QUdpSocket mSocket;
    QHBoxLayout* mLayout;

    void setupButton(QWidget* parent);

private slots:
    void sendCommand();

};

#endif // BUTTON_H
