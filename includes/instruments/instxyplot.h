#ifndef INSTXYPLOT_H
#define INSTXYPLOT_H

#include "visuinstrument.h"

class InstXYPlot : public VisuInstrument
{
    Q_OBJECT

public:
    explicit InstXYPlot(
            QWidget *parent,
            QMap<QString, QString> properties,
            QMap<QString, VisuPropertyMeta> metaProperties) : VisuInstrument(parent, properties, metaProperties)
    {
        loadProperties(mProperties);

        mSignalX = nullptr;
        mSignalY = nullptr;
    }
    static const QString TAG_NAME;

    virtual bool updateProperties(const QString &key, const QString &value);
    void loadProperties(QMap<QString, QString>& properties)
    {
        VisuInstrument::loadProperties(properties);

        GET_PROPERTY(cSignalIdY, quint16, properties, mPropertiesMeta);
        GET_PROPERTY(cBallSize, quint16, properties, mPropertiesMeta);
        GET_PROPERTY(cMajorCntX, quint16, properties, mPropertiesMeta);
        GET_PROPERTY(cMajorCntY, quint16, properties, mPropertiesMeta);
        GET_PROPERTY(cMajorLen, quint16, properties, mPropertiesMeta);
        GET_PROPERTY(cLabelX, QString, properties, mPropertiesMeta);
        GET_PROPERTY(cLabelY, QString, properties, mPropertiesMeta);
        GET_PROPERTY(cPadding, quint16, properties, mPropertiesMeta);
        GET_PROPERTY(cDecimals, quint8, properties, mPropertiesMeta);

        mTagName = InstXYPlot::TAG_NAME;
    }

    static const int SIGNAL_FIRST = 0;
    static const int SIGNAL_SECOND = 1;
    static const int MARGIN = 2;

private:
    // configuration properties
    quint16 cSignalIdY;
    quint16 cBallSize;
    quint16 cMajorCntX;
    quint16 cMajorCntY;
    quint16 cMajorLen;
    QString cLabelX;
    QString cLabelY;
    quint16 cPadding;
    quint8 cDecimals;

    // aux properties
    double mLastValX;
    double mLastValY;
    double mCenterX;
    double mCenterY;

    const VisuSignal *mSignalX;
    const VisuSignal *mSignalY;

    void renderSingleAxis(QPainter* painter, int sigInd, int divisions, int length);
    void renderAxis(QPainter* painter);
    void renderBall(QPainter* painter);

protected:
    virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
    virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTXYPLOT_H
