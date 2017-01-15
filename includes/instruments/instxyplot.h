#ifndef INSTXYPLOT_H
#define INSTXYPLOT_H

#include "visuinstrument.h"

class InstXYPlot : public VisuInstrument
{
    Q_OBJECT

public:
    explicit InstXYPlot(
            QWidget *parent,
            QMap<QString, QString> properties) : VisuInstrument(parent, properties)
    {
        loadProperties(properties);
    }
    static const QString TAG_NAME;
    quint16 getSignalYId();

    virtual void loadProperties(QMap<QString, QString> properties)
    {
        VisuInstrument::loadProperties(properties);

        GET_PROPERTY(cSignalIdY, quint16, properties);
        GET_PROPERTY(cBallSize, quint16, properties);
        GET_PROPERTY(cMajorCnt, quint16, properties);
        GET_PROPERTY(cMajorLen, quint16, properties);
        GET_PROPERTY(cLabelX, QString, properties);
        GET_PROPERTY(cLabelY, QString, properties);

        mTagName = InstXYPlot::TAG_NAME;
    }

private:
    // configuration properties
    quint16 cSignalIdY;
    quint16 cBallSize;
    quint16 cMajorCnt;
    quint16 cMajorLen;
    QString cLabelX;
    QString cLabelY;

    // aux properties
    double mLastValX;
    double mLastValY;
    double mCenterX;
    double mCenterY;

    void renderAxis(QPainter* painter);
    void renderBall(QPainter* painter);

protected:
    virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
    virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTXYPLOT_H
