#include "instlinear.h"

const QString InstLinear::TAG_NAME = "LINEAR";

void InstLinear::renderLabel(QPainter* painter, int sigCur, quint16 xOfs)
{
    QString label = QString("%1").arg((int)sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    double labelWidth = fontMetrics.width(label);
    double labelHeight = fontMetrics.height();
    painter->drawText(xOfs - labelWidth / 2, height - ver_margin + labelHeight, label);
}

void InstLinear::renderDivisions(QPainter* painter)
{
    quint16 total = major_cnt * minor_cnt;
    quint16 delta = (width - 2 * hor_margin) / total;
    quint16 xOfs = hor_margin;
    quint16 tmpMargin;
    double sigMax = mSignal->getMax();
    double sigMin = mSignal->getMin();
    double sigStep = (sigMax - sigMin) / (major_cnt);
    double sigCur = sigMin;

    for (int i=0; i<=total; ++i)
    {
        if (i % minor_cnt == 0)
        {
            renderLabel(painter, sigCur, xOfs);
            sigCur += sigStep;
            tmpMargin = ver_margin;
        }
        else
        {
            tmpMargin = ver_minor_margin;
        }

        painter->drawLine(xOfs, ver_margin, xOfs, height - tmpMargin);
        xOfs += delta;
    }

    mBarLength = xOfs - delta - hor_margin;
}

void InstLinear::renderStatic(QPainter *painter)
{
    clear(painter);

    setPen(painter, color_static, line_thickness);
    renderDivisions(painter);

    painter->drawLine(hor_margin, ver_margin, mBarLength + hor_margin, ver_margin);
}

void InstLinear::renderDynamic(QPainter *painter)
{
    setPen(painter, color_static);
    setBrush(painter, color_foreground);

    double xOfs = mSignal->getNormalizedValue() * mBarLength;
    painter->drawRect(hor_margin, 5, xOfs, ver_margin - 10);
}
