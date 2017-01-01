#ifndef INSTLINEAR_H
#define INSTLINEAR_H

#include "visuinstrument.h"
#include <QPainter>

class InstLinear : public VisuInstrument
{
    Q_OBJECT
    public:
        InstLinear(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            GET_PROPERTY(line_thickness, quint8);
            GET_PROPERTY(major_len, quint8);
            GET_PROPERTY(minor_len, quint8);
            GET_PROPERTY(major_cnt, quint8);
            GET_PROPERTY(minor_cnt, quint8);
            GET_PROPERTY(font_size, quint8);
            GET_PROPERTY(hor_margin, quint16);
            GET_PROPERTY(ver_margin, quint16);
            GET_PROPERTY(ver_minor_margin, quint16);
        }
        static const QString TAG_NAME;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap

    private:
        quint8 line_thickness;  // Thickness of geometric primitives drawn
        quint8 major_len;       // Length, in pixels, of major division marks
        quint8 minor_len;       // Length, in pixels, of minor division marks
        quint8 major_cnt;       // Number of major count divisions
        quint8 minor_cnt;       // Number of minor count divisions
        quint8 font_size;       // Size of font used on labels
        quint16 hor_margin;
        quint16 ver_margin;
        quint16 ver_minor_margin;

        // additional properties not related to configuration
        quint16 mBarLength;

        void renderDivisions(QPainter* painter);
        void renderLabel(QPainter* painter, int sigCur, quint16 xOfs);

};

#endif // INSTLINEAR_H
