#include "visumisc.h"
#include "visuhelper.h"

void VisuMisc::setBackgroundColor(QWidget* widget, QColor color)
{
    QString stylesheet = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    widget->setStyleSheet(stylesheet);
}

QString VisuMisc::getXMLDeclaration()
{
    return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
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
