#ifndef INSTLINEAR_H
#define INSTLINEAR_H

#include "visuinstrument.h"

class InstLinear : public VisuInstrument
{
    Q_OBJECT
    public:
        InstLinear(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            loadProperties(properties);
        }
        static const QString TAG_NAME;

        virtual void loadProperties(QMap<QString, QString> properties)
        {
            VisuInstrument::loadProperties(properties);

            GET_PROPERTY(cLineThickness, quint8, properties);
            GET_PROPERTY(cMajorLen, quint8, properties);
            GET_PROPERTY(cMinorLen, quint8, properties);
            GET_PROPERTY(cMajorCnt, quint8, properties);
            GET_PROPERTY(cMinorCnt, quint8, properties);
            GET_PROPERTY(cHorMargin, quint16, properties);
            GET_PROPERTY(cVerMargin, quint16, properties);
            GET_PROPERTY(cVerMinorMargin, quint16, properties);
            GET_PROPERTY(cHorizontal, bool, properties);

            mTagName = InstLinear::TAG_NAME;
        }

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
        virtual bool refresh(const QString &key);

    private:
        // configuration properties
        quint8 cLineThickness;  // Thickness of geometric primitives drawn
        quint8 cMajorLen;       // Length, in pixels, of major division marks
        quint8 cMinorLen;       // Length, in pixels, of minor division marks
        quint8 cMajorCnt;       // Number of major count divisions
        quint8 cMinorCnt;       // Number of minor count divisions
        quint16 cHorMargin;
        quint16 cVerMargin;
        quint16 cVerMinorMargin;
        bool cHorizontal;

        // additional properties not related to configuration
        quint16 mBarLength;

        void renderDivisions(QPainter* painter);
        void renderLabel(QPainter* painter, int sigCur, quint16 xOfs);

        static const QString KEY_HORIZONTAL;

};

#endif // INSTLINEAR_H
