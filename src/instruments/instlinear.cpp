#include "instlinear.h"

const QString InstLinear::TAG_NAME = "LINEAR";

void InstLinear::renderLabel(QPainter* painter, int sigCur, quint16 xOfs)
{
    QString label = QString("%1").arg((int)sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    double labelWidth = fontMetrics.width(label);
    double labelHeight = fontMetrics.height();
    painter->drawText(xOfs - labelWidth / 2, cHeight - cVerMargin + labelHeight, label);
}

void InstLinear::renderDivisions(QPainter* painter)
{
    quint16 total = cMajorCnt * cMinorCnt;
    quint16 delta = (cWidth - 2 * cHorMargin) / total;
    quint16 xOfs = cHorMargin;
    quint16 tmpMargin;
    double sigMax = mSignal->getMax();
    double sigMin = mSignal->getMin();
    double sigStep = (sigMax - sigMin) / (cMajorCnt);
    double sigCur = sigMin;

    for (int i=0; i<=total; ++i)
    {
        if (i % cMinorCnt == 0)
        {
            renderLabel(painter, sigCur, xOfs);
            sigCur += sigStep;
            tmpMargin = cVerMargin;
        }
        else
        {
            tmpMargin = cVerMinorMargin;
        }

        painter->drawLine(xOfs, cVerMargin, xOfs, cHeight - tmpMargin);
        xOfs += delta;
    }

    mBarLength = xOfs - delta - cHorMargin;
}

void InstLinear::renderStatic(QPainter *painter)
{
    clear(painter);

    setPen(painter, cColorStatic, cLineThickness);
    setFont(painter, cFontSize);
    renderDivisions(painter);

    painter->drawLine(cHorMargin, cVerMargin, mBarLength + cHorMargin, cVerMargin);
}

void InstLinear::renderDynamic(QPainter *painter)
{
    setPen(painter, cColorStatic);
    setBrush(painter, cColorForeground);

    double xOfs = mSignal->getNormalizedValue() * mBarLength;
    painter->drawRect(cHorMargin, 5, xOfs, cVerMargin - 10);
}
