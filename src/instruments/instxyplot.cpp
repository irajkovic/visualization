#include "instxyplot.h"

const QString InstXYPlot::TAG_NAME = "XY_PLOT";

InstXYPlot::InstXYPlot()
{


}

void InstXYPlot::renderAxis(QPainter* painter)
{
    int dx = width / major_cnt;
    int dy = height / major_cnt;
    int x = 0;
    int y = 0;
    for (int i=0; i<major_cnt + 1; ++i)
    {
        painter->drawLine(x, mCenterY-major_len, x, mCenterY+major_len);
        painter->drawLine(mCenterX-major_len, y, mCenterX+major_len, y);
        x += dx;
        y += dy;
    }
}

void InstXYPlot::renderBall(QPainter* painter)
{
    setPen(painter, color_static);
    setBrush(painter, color_foreground);
    QRect rect(lastValX * (width-ball_size), lastValY * (height-ball_size), ball_size, ball_size);
    painter->drawEllipse(rect);
}

void InstXYPlot::renderStatic(QPainter *painter)
{
    painter->fillRect(0, 0, width, height, color_background);

    mCenterX = width / 2;
    mCenterY = height / 2;

    painter->drawLine(0, mCenterY, width, mCenterY);
    painter->drawLine(mCenterX, 0, mCenterX, height);

    renderAxis(painter);

    painter->drawText(0, mCenterY-major_len, label_x);
    painter->drawText(mCenterX+major_len, height, label_y);
}

void InstXYPlot::renderDynamic(QPainter *painter)
{
    if (mSignal->getId() == signal_id)
    {
        lastValX = mSignal->getNormalizedValue();   // primary signal shown on X axis
    }
    else
    {
        lastValY = mSignal->getNormalizedValue();   // additional signal shown on Y axis
    }

    renderBall(painter);
}

quint16 InstXYPlot::getSignalYId()
{
    return signal_y_id;
}
