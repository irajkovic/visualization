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
        GET_PROPERTY(signal_y_id, quint16, properties);
        GET_PROPERTY(ball_size, quint16, properties);
        GET_PROPERTY(major_cnt, quint16, properties);
        GET_PROPERTY(major_len, quint16, properties);
        GET_PROPERTY(label_x, QString, properties);
        GET_PROPERTY(label_y, QString, properties);
    }
    static const QString TAG_NAME;
    quint16 getSignalYId();

private:
    quint16 signal_y_id;
    quint16 ball_size;
    quint16 major_cnt;
    quint16 major_len;
    QString label_x;
    QString label_y;

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
