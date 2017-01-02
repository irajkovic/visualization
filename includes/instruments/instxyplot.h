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
        GET_PROPERTY(cSignalYId, quint16, properties);
        GET_PROPERTY(cBallSize, quint16, properties);
        GET_PROPERTY(cMajorCnt, quint16, properties);
        GET_PROPERTY(cMajorLen, quint16, properties);
        GET_PROPERTY(cLabelX, QString, properties);
        GET_PROPERTY(cLabelY, QString, properties);
    }
    static const QString TAG_NAME;
    quint16 getSignalYId();

private:
    quint16 cSignalYId;
    quint16 cBallSize;
    quint16 cMajorCnt;
    quint16 cMajorLen;
    QString cLabelX;
    QString cLabelY;

    // last values for X and Y signals
    double lastValX;
    double lastValY;
    double mCenterX;
    double mCenterY;

    void renderAxis(QPainter* painter);
    void renderBall(QPainter* painter);

protected:
    virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
    virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTXYPLOT_H
