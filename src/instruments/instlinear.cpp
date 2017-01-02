#include "instlinear.h"

const QString InstLinear::TAG_NAME = "LINEAR";

void InstLinear::renderLabel(QPainter* painter, int sigCur, quint16 xOfs)
{
    QString label = QString("%1").arg((int)sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    double labelWidth = fontMetrics.width(label);
    double labelHeight = fontMetrics.height();
    painter->drawText(xOfs - labelWidth / 2, height - verMargin + labelHeight, label);
}

void InstLinear::renderDivisions(QPainter* painter)
{
    quint16 total = majorCnt * minorCnt;
    quint16 delta = (width - 2 * horMargin) / total;
    quint16 xOfs = horMargin;
    quint16 tmpMargin;
    double sigMax = mSignal->getMax();
    double sigMin = mSignal->getMin();
    double sigStep = (sigMax - sigMin) / (majorCnt);
    double sigCur = sigMin;

    for (int i=0; i<=total; ++i)
    {
        if (i % minorCnt == 0)
        {
            renderLabel(painter, sigCur, xOfs);
            sigCur += sigStep;
            tmpMargin = verMargin;
        }
        else
        {
            tmpMargin = verMinorMargin;
        }

        painter->drawLine(xOfs, verMargin, xOfs, height - tmpMargin);
        xOfs += delta;
    }

    mBarLength = xOfs - delta - horMargin;
}

void InstLinear::renderStatic(QPainter *painter)
{
    clear(painter);

    setPen(painter, colorStatic, lineThickness);
    renderDivisions(painter);

    painter->drawLine(horMargin, verMargin, mBarLength + horMargin, verMargin);
}

void InstLinear::renderDynamic(QPainter *painter)
{
    setPen(painter, colorStatic);
    setBrush(painter, colorForeground);

    double xOfs = mSignal->getNormalizedValue() * mBarLength;
    painter->drawRect(horMargin, 5, xOfs, verMargin - 10);
}
