#ifndef INSTLED_H
#define INSTLED_H

#include "visuinstrument.h"

class InstLED : public VisuInstrument
{
    Q_OBJECT
public:

    enum CONDITION
    {
        BETWEEN     = 0,
        LESS_THAN   = 1,
        LESS_EQUAL  = 2,
        MORE_EQUAL  = 3,
        MORE_THAN   = 4
    };

    explicit InstLED(
            QWidget *parent,
            QMap<QString, QString> properties) : VisuInstrument(parent, properties)
    {
        GET_PROPERTY(radius, quint8);
        GET_PROPERTY(val1, double);
        GET_PROPERTY(val2, double);
        GET_PROPERTY(condition, quint8);
        GET_PROPERTY(color_on, QColor);
        GET_PROPERTY(color_off, QColor);
        GET_PROPERTY(show_signal_lbl, quint8);
        GET_PROPERTY(font_size, quint8);
    }
    static const QString TAG_NAME;

private:

    quint8 radius;
    double val1;
    double val2;
    quint8 condition;
    QColor color_on;
    QColor color_off;
    quint8 show_signal_lbl;
    quint8 font_size;

    // last values for X and Y signals
    double lastValX;
    double lastValY;
    double mCenterX;
    double mCenterY;

protected:
    virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
    virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};






#endif // INSTLED_H
