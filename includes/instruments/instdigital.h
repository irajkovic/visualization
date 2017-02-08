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
            loadProperties(properties);
        }
        static const QString TAG_NAME;

        virtual void loadProperties(QMap<QString, QString> properties)
        {
            VisuInstrument::loadProperties(properties);

            GET_PROPERTY(cFontSize, quint8, properties);
            GET_PROPERTY(cShowSignalName, bool, properties);
            GET_PROPERTY(cShowSignalUnit, bool, properties);
            GET_PROPERTY(cPadding, quint8, properties);

            mTagName = InstDigital::TAG_NAME;
        }

    private:
        // configuration properties
        quint8 cFontSize;
        bool cShowSignalName;
        bool cShowSignalUnit;
        quint8 cPadding;

        // aux members
        QFont mFont;
        QString mFormat;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTDIGITAL_H
