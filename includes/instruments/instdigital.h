#ifndef INSTDIGITAL_H
#define INSTDIGITAL_H

#include "visuinstrument.h"

class InstDigital : public VisuInstrument
{
    Q_OBJECT
    public:
        explicit InstDigital(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            GET_PROPERTY(cFontSize, quint8, properties);
            GET_PROPERTY(cShowSignalName, quint8, properties);
            GET_PROPERTY(cShowSignalUnit, quint8, properties);
            GET_PROPERTY(cPadding, quint8, properties);
        }
        static const QString TAG_NAME;

    private:
        // configuration properties
        quint8 cFontSize;
        quint8 cShowSignalName;
        quint8 cShowSignalUnit;
        quint8 cPadding;

        // aux members
        QFont mFont;
        QString mFormat;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTDIGITAL_H
