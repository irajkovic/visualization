#ifndef INSTDIGITAL_H
#define INSTDIGITAL_H

#include "visuinstrument.h"

class InstDigital : public VisuInstrument
{
    Q_OBJECT
    public:
        explicit InstDigital(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            GET_PROPERTY(fontSize, quint8, properties);
            GET_PROPERTY(showSignalName, quint8, properties);
            GET_PROPERTY(showSignalUnit, quint8, properties);
            GET_PROPERTY(padding, quint8, properties);
        }
        static const QString TAG_NAME;

    private:
        // custom properties
        quint8 fontSize;
        quint8 showSignalName;
        quint8 showSignalUnit;
        quint8 padding;

        // internal members
        QFont font;
        QString format;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTDIGITAL_H
