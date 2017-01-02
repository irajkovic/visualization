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
            GET_PROPERTY(lineThickness, quint8, properties);
            GET_PROPERTY(majorLen, quint8, properties);
            GET_PROPERTY(minorLen, quint8, properties);
            GET_PROPERTY(majorCnt, quint8, properties);
            GET_PROPERTY(minorCnt, quint8, properties);
            GET_PROPERTY(fontSize, quint8, properties);
            GET_PROPERTY(horMargin, quint16, properties);
            GET_PROPERTY(verMargin, quint16, properties);
            GET_PROPERTY(verMinorMargin, quint16, properties);
        }
        static const QString TAG_NAME;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap

    private:
        quint8 lineThickness;  // Thickness of geometric primitives drawn
        quint8 majorLen;       // Length, in pixels, of major division marks
        quint8 minorLen;       // Length, in pixels, of minor division marks
        quint8 majorCnt;       // Number of major count divisions
        quint8 minorCnt;       // Number of minor count divisions
        quint8 fontSize;       // Size of font used on labels
        quint16 horMargin;
        quint16 verMargin;
        quint16 verMinorMargin;

        // additional properties not related to configuration
        quint16 mBarLength;

        void renderDivisions(QPainter* painter);
        void renderLabel(QPainter* painter, int sigCur, quint16 xOfs);

};

#endif // INSTLINEAR_H
