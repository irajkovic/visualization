#include "instlinear.h"

const QString InstLinear::TAG_NAME = "LINEAR";
const QString InstLinear::KEY_HORIZONTAL = "horizontal";

void InstLinear::renderLabel(QPainter* painter, int sigCur, quint16 ofs)
{
    QString label = QString("%1").arg((int)sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    double labelWidth = fontMetrics.width(label);
    double labelHeight = fontMetrics.height();
    if (cHorizontal)
    {
        painter->drawText(ofs - labelWidth / 2, cHeight - cVerMargin + labelHeight, label);
    }
    else
    {
        painter->drawText(cWidth - cHorMargin, ofs + labelHeight / 2, label);
    }
}

void InstLinear::renderDivisions(QPainter* painter)
{
    quint16 total = cMajorCnt * cMinorCnt;
    double delta;
    double ofs;

    if (cHorizontal)
    {
        delta = (double)(cWidth - 2 * cHorMargin) / total;
        ofs = cHorMargin;
    }
    else
    {
        delta = (double)(cHeight - 2 * cVerMargin) / total;
        ofs = cVerMargin;
    }

    quint16 tmpMargin;
    double sigMax = mSignal->getMax();
    double sigMin = mSignal->getMin();
    double sigStep = (sigMax - sigMin) / (cMajorCnt);
    double sigCur = sigMin;

    if (!cHorizontal)
    {
        sigStep = -sigStep;
        sigCur = sigMax;
    }

    for (int i=0; i<=total; ++i)
    {
        if (i % cMinorCnt == 0)
        {
            renderLabel(painter, sigCur, ofs);
            sigCur += sigStep;
            tmpMargin = cVerMargin;
        }
        else
        {
            tmpMargin = cVerMinorMargin;
        }

        if (cHorizontal)
        {
            painter->drawLine(ofs, cVerMargin, ofs, cHeight - tmpMargin);
        }
        else
        {
            painter->drawLine(cHorMargin, ofs, cWidth - tmpMargin, ofs);
        }

        ofs += delta;
    }

    if (cHorizontal)
    {
        mBarLength = ofs - delta - cHorMargin;
    }
    else
    {
        mBarLength = ofs - delta - cVerMargin;
    }

}

void InstLinear::renderStatic(QPainter *painter)
{
    clear(painter);

    setPen(painter, cColorStatic, cLineThickness);
    setFont(painter);
    renderDivisions(painter);

    if (cHorizontal)
    {
        painter->drawLine(cHorMargin, cVerMargin, mBarLength + cHorMargin, cVerMargin);
    }
    else
    {
        painter->drawLine(cHorMargin, cVerMargin, cHorMargin, cVerMargin + mBarLength);
    }

}

void InstLinear::renderDynamic(QPainter *painter)
{
    setPen(painter, cColorStatic);
    setBrush(painter, cColorForeground);

    double ofs = mSignal->getNormalizedValue() * mBarLength;
    if (cHorizontal)
    {
        painter->drawRect(cHorMargin, 5, ofs, cVerMargin - 10);
    }
    else
    {
        painter->drawRect(5, cHeight - cVerMargin, cHorMargin - 10,  -ofs);

        qDebug("%d %f %d", cHeight, ofs, mBarLength);
    }
}

bool InstLinear::refresh(const QString& key)
{
    bool changed = false;
    if (key == KEY_HORIZONTAL)
    {
        // update dimensions as well
        std::swap(cWidth, cHeight);
        std::swap(mProperties[KEY_WIDTH], mProperties[KEY_HEIGHT]);
        setup();
        changed = true;
    }

    VisuInstrument::refresh(key);

    return changed;
}
