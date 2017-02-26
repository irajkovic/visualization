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
            QMap<QString, QString> properties,
            QMap<QString, VisuPropertyMeta> metaProperties) : VisuInstrument(parent, properties, metaProperties)
    {
        loadProperties();
    }
    static const QString TAG_NAME;

    virtual bool updateProperties(const QString &key, const QString &value);
    void loadProperties();

private:
    // configuration properties
    quint8 cRadius;
    double cVal1;
    double cVal2;
    quint8 cCondition;
    QColor cColorOn;
    QColor cColorOff;
    int cImageOn;
    int cImageOff;
    quint8 cShowSignalName;
    quint16 cCenterH;

    // aux properties
    double mLastValX;
    double mLastValY;
    double mCenterX;
    double mCenterY;

protected:
    virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
    virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};






#endif // INSTLED_H
