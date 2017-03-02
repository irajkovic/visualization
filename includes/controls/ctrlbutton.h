#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QUdpSocket>
#include <QHBoxLayout>
#include <QColor>
#include "visupropertyloader.h"
#include "visucontrol.h"

class CtrlButton : public VisuControl
{
    public:
        explicit CtrlButton(
                QWidget *parent,
                QMap<QString, QString> properties,
                QMap<QString, VisuPropertyMeta> metaProperties) : VisuControl(parent, properties, metaProperties)
        {

            loadProperties();
            setup(parent);
        }

        static const QString TAG_NAME;

        virtual bool updateProperties(const QString &key, const QString &value);
        void loadProperties();
        virtual bool refresh(const QString& key);
        void setup(QWidget* parent);

private:

    Q_OBJECT

    QString cActionMessage;
    QString cCss;
    QPushButton* mButton;
    QHBoxLayout* mLayout;
    QColor cColorBackground;
    QColor cColorForeground;
    QColor cColorBorder;
    quint32 cBorderRadius;
    quint16 cFontSize;
    QString cFontType;
    quint8 cBorderThickness;

    void setupButton(QWidget* parent);
    QString generateCss();

private slots:
    void sendCommand();

};

#endif // BUTTON_H
