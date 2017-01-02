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
        GET_PROPERTY(radius, quint8, properties);
        GET_PROPERTY(val1, double, properties);
        GET_PROPERTY(val2, double, properties);
        GET_PROPERTY(condition, quint8, properties);
        GET_PROPERTY(colorOn, QColor, properties);
        GET_PROPERTY(colorOff, QColor, properties);
        GET_PROPERTY(showSignalName, quint8, properties);
        GET_PROPERTY(fontSize, quint8, properties);
    }
    static const QString TAG_NAME;

private:

    quint8 radius;
    double val1;
    double val2;
    quint8 condition;
    QColor colorOn;
    QColor colorOff;
    quint8 showSignalName;
    quint8 fontSize;

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
