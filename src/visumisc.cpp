#include "visumisc.h"
#include "visupropertyloader.h"

void VisuMisc::setBackgroundColor(QWidget* widget, QColor color)
{
    QString stylesheet = QString("background-color: %1;").arg(VisuMisc::colorToStr(color));
    widget->setStyleSheet(stylesheet);
}

QString VisuMisc::getXMLDeclaration()
{
    return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

QString VisuMisc::addElement(QString tag, QMap<QString, QString> properties, int tabs)
{
    QString xml = VisuMisc::openTag(tag, tabs);
    xml += VisuMisc::mapToString(properties, tabs + 1);
    xml += VisuMisc::closeTag(tag, tabs);
    return xml;
}

QString VisuMisc::openTag(QString tag, int tabs)
{
    return QString("\t").repeated(tabs) + "<" + tag + ">\n";
}

QString VisuMisc::closeTag(QString tag, int tabs)
{
    return QString("\t").repeated(tabs) + "</" + tag + ">\n";
}

QString VisuMisc::mapToString(QMap<QString, QString> properties, int tabs)
{
    QString xml;
    QString whitespace = QString("\t").repeated(tabs);
    for (auto i = properties.begin(); i != properties.end(); ++i)
    {
        xml += whitespace + "<" + i.key() + ">";
        xml += i.value();
        xml += "</" + i.key() + ">\n";
    }

    return xml;
}

QString VisuMisc::saveToFile(QFile &file, QString contents)
{
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );
        stream << contents;
        file.close();
    }
    return file.fileName();
}

QPen VisuMisc::getDashedPen(QColor color, int thickness)
{
    QPen dashed;
    dashed.setStyle(Qt::DashLine);
    dashed.setColor(color);
    dashed.setWidth(thickness);
    return dashed;
}

QColor VisuMisc::strToColor(const QString& str)
{
    QStringList parts = str.split(",");
    if (parts.length() == 4)
    {
        return QColor( parts[0].toInt()
                     , parts[1].toInt()
                     , parts[2].toInt()
                     , parts[3].toInt());
    }
    return QColor::Invalid;
}

QString VisuMisc::colorToStr(const QColor& color)
{
    QString colorStr = QString("rgba(%1, %2, %3, %4)")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
            .arg((double)color.alpha()/255.0);
    return colorStr;
}

QImage VisuMisc::strToImage(const QString& str, const QString& format)
{
    QByteArray base64Bytes;
    base64Bytes.append(str);

    QByteArray rawImageData;
    rawImageData = QByteArray::fromBase64(base64Bytes);

    QImage image;
    image.loadFromData(rawImageData, format.toStdString().c_str());

    return image;
}
