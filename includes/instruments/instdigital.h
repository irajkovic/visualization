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
            loadProperties(mProperties);
        }
        static const QString TAG_NAME;

        virtual bool updateProperties(const QString &key, const QString &value);
        void loadProperties(QMap<QString, QString>& properties)
        {
            VisuInstrument::loadProperties(properties);

            GET_PROPERTY(cShowSignalName, bool, properties, mPropertiesMeta);
            GET_PROPERTY(cShowSignalUnit, bool, properties, mPropertiesMeta);
            GET_PROPERTY(cPadding, quint8, properties, mPropertiesMeta);
            GET_PROPERTY(cLeadingDigits, quint8, properties, mPropertiesMeta);

            mTagName = InstDigital::TAG_NAME;
        }

    private:
        // configuration properties
        bool cShowSignalName;
        bool cShowSignalUnit;
        quint8 cPadding;
        quint8 cLeadingDigits;

        // aux members
        QFont mFont;
        QString mFormat;

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTDIGITAL_H
