#ifndef INSTDIGITAL_H
#define INSTDIGITAL_H

#include "visuinstrument.h"

class InstDigital : public VisuInstrument
{
    Q_OBJECT
    public:
        explicit InstDigital(
                QWidget *parent,
                QMap<QString, QString> properties,
                QMap<QString, VisuPropertyMeta> metaProperties) : VisuInstrument(parent, properties, metaProperties)
        {
            loadProperties();
        }
        static const QString TAG_NAME;

        virtual bool updateProperties(const QString &key, const QString &value);
        void loadProperties();

    private:
        // configuration properties
        bool cShowSignalName;
        bool cShowSignalUnit;
        quint8 cPadding;
        quint8 cLeadingDigits;
        quint8 cDecimalDigits;

        // aux members
        QFont mFont;
        QString mFormat;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTDIGITAL_H
