#ifndef INSTDIGITAL_H
#define INSTDIGITAL_H

#include "instrument.h"

class InstDigital : public VisuInstrument
{
    public:
        InstDigital();
        explicit InstDigital(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            GET_PROPERTY(font_size, quint8);
            GET_PROPERTY(show_signal_name, quint8);
            GET_PROPERTY(show_signal_unit, quint8);
            GET_PROPERTY(padding, quint8);
        }
        static const QString TAG_NAME;

    private:
        // custom properties
        quint8 font_size;
        quint8 show_signal_name;
        quint8 show_signal_unit;
        quint8 padding;

        // internal members
        QFont font;
        QString format;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTDIGITAL_H
