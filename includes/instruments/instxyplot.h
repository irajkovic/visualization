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
        GET_PROPERTY(signalYId, quint16, properties);
        GET_PROPERTY(ballSize, quint16, properties);
        GET_PROPERTY(majorCnt, quint16, properties);
        GET_PROPERTY(majorLen, quint16, properties);
        GET_PROPERTY(labelX, QString, properties);
        GET_PROPERTY(labelY, QString, properties);
    }
    static const QString TAG_NAME;
    quint16 getSignalYId();

private:
    quint16 signalYId;
    quint16 ballSize;
    quint16 majorCnt;
    quint16 majorLen;
    QString labelX;
    QString labelY;

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
