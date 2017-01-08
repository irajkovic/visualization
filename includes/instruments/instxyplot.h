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
        load(properties);
    }
    static const QString TAG_NAME;
    quint16 getSignalYId();

    virtual void load(QMap<QString, QString> properties)
    {
        VisuInstrument::load(properties);

        GET_PROPERTY(cSignalYId, quint16, properties);
        GET_PROPERTY(cBallSize, quint16, properties);
        GET_PROPERTY(cMajorCnt, quint16, properties);
        GET_PROPERTY(cMajorLen, quint16, properties);
        GET_PROPERTY(cLabelX, QString, properties);
        GET_PROPERTY(cLabelY, QString, properties);
    }

private:
    // configuration properties
    quint16 cSignalYId;
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
