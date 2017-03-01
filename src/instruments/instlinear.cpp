#include "instlinear.h"

const QString InstLinear::TAG_NAME = "LINEAR";
const QString InstLinear::KEY_HORIZONTAL = "horizontal";

bool InstLinear::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstLinear::loadProperties();
    return InstLinear::refresh(key);
}

void InstLinear::loadProperties()
{
    VisuInstrument::loadProperties();

    GET_PROPERTY(cLineThickness, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorLen, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMinorLen, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorCnt, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMinorCnt, mProperties, mPropertiesMeta);
    GET_PROPERTY(cBarThickness, mProperties, mPropertiesMeta);
    GET_PROPERTY(cHorizontal, mProperties, mPropertiesMeta);

    mTagName = InstLinear::TAG_NAME;
}

void InstLinear::renderLabel(QPainter* painter, int sigCur, quint16 ofs)
{
    QString label = QString("%1").arg((int)sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    double labelWidth = fontMetrics.width(label);
    double labelHeight = fontMetrics.height();
    if (cHorizontal)
    {
        painter->drawText(ofs - labelWidth / 2, cBarThickness + cMajorLen + 2 * SPACING + labelHeight, label);
    }
    else
    {
        painter->drawText(cBarThickness + cMajorLen + 3 * SPACING, ofs + labelHeight / 2, label);
    }
}

void InstLinear::renderDivisions(QPainter* painter)
{
    quint16 total = cMajorCnt * cMinorCnt;
    int directionDimension = cHorizontal ? cWidth : cHeight;
    double delta = (double)(directionDimension - 2 * mMargin) / total;
    double ofs = mMargin;

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
            tmpMargin = cMajorLen;
        }
        else
        {
            tmpMargin = cMinorLen;
        }

        if (cHorizontal)
        {
            painter->drawLine(ofs, cBarThickness + 2 * SPACING, ofs,  cBarThickness + 2 * SPACING + tmpMargin);
        }
        else
        {
            painter->drawLine(cBarThickness + 2 * SPACING, ofs, cBarThickness + 2 * SPACING + tmpMargin, ofs);
        }

        ofs += delta;
    }


    mBarLength = ofs - delta - mMargin;
}

void InstLinear::setupMargin(QPainter* painter)
{
    QFontMetrics fontMetrics = painter->fontMetrics();

    if (cHorizontal)
    {
        int minValueWidth = fontMetrics.width(QString("%1").arg(mSignal->getMin()));
        int maxValueWidth = fontMetrics.width(QString("%1").arg(mSignal->getMax()));
        mMargin = std::max(minValueWidth, maxValueWidth) / 2;
    }
    else
    {
        mMargin = fontMetrics.height() / 2;
    }
}

void InstLinear::renderStatic(QPainter *painter)
{
    clear(painter);

    setPen(painter, cColorStatic, cLineThickness);
    setFont(painter);
    setupMargin(painter);
    renderDivisions(painter);

    if (cHorizontal)
    {
        painter->drawLine(mMargin, cBarThickness + 2 * SPACING, mBarLength + mMargin, cBarThickness + 2 * SPACING);
    }
    else
    {
        painter->drawLine(cBarThickness + 2 * SPACING, mMargin, cBarThickness + 2 * SPACING, mMargin + mBarLength);
    }

}

void InstLinear::renderDynamic(QPainter *painter)
{
    setPen(painter, cColorStatic);
    setBrush(painter, cColorForeground);

    double ofs = mSignal->getNormalizedValue() * mBarLength;
    if (cHorizontal)
    {
        painter->drawRect(mMargin, SPACING, ofs, cBarThickness);
    }
    else
    {
        painter->drawRect(SPACING, cHeight - mMargin, cBarThickness, -ofs);
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
