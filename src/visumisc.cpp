#include "visumisc.h"

#include <QPalette>
#include <QPushButton>
#include <QColorDialog>
#include "visuhelper.h"

const char* VisuMisc::PROP_COLOR = "color";
const char* VisuMisc::PROP_ROW = "row";
const char* VisuMisc::PROP_X = "x";
const char* VisuMisc::PROP_Y = "y";
const char* VisuMisc::PROP_SIGNAL_ID = "signalId";

bool VisuMisc::isColorProperty(QString property)
{
    return property.contains(VisuMisc::PROP_COLOR);
}

void VisuMisc::setBackgroundColor(QWidget* widget, QColor color)
{
    QString stylesheet = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    widget->setStyleSheet(stylesheet);
}

void VisuMisc::updateTable(QTableWidget* table, QMap<QString, QString> properties, QWidget* object, const char* slot)
{
    table->clearContents();
    table->setRowCount(properties.size());
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList{"Property", "Value"});

    int row = 0;
    for (auto i = properties.begin(); i != properties.end(); ++i)
    {
        //mPropertiesTable->setCellWidget(row, 0, new QLabel(i.key()) );
        //mPropertiesTable->setCellWidget(row, 1, new QLineEdit(i.value()) );

        QString key = i.key();
        QString value = i.value();
        table->setItem(row, 0, new QTableWidgetItem(key));

        if (VisuMisc::isColorProperty(key))
        {
            QColor color = VisuHelper::get<QColor>(key, properties);
            QPushButton* btn = new QPushButton(i.value());
            btn->setProperty(VisuMisc::PROP_COLOR, color);
            btn->setProperty(VisuMisc::PROP_ROW, row);
            VisuMisc::setBackgroundColor(btn, color);
            table->setCellWidget(row, 1, btn);
            if (object != nullptr && slot != nullptr)
            {
                QObject::connect(btn, SIGNAL(clicked()), object, slot);
            }
        }
        else
        {
            table->setItem(row, 1, new QTableWidgetItem(value));
        }

        ++row;
    }
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

int VisuMisc::updateColor(QObject* sender, QWidget* parent)
{
    QPushButton* btn = static_cast<QPushButton*>(sender);
    QColor oldColor = btn->property(VisuMisc::PROP_COLOR).value<QColor>();
    int row = btn->property(VisuMisc::PROP_ROW).toInt();

    QColor newColor = QColorDialog::getColor(oldColor,
                                             parent,
                                             QObject::tr("Set color"),
                                             QColorDialog::ShowAlphaChannel);

    QString colorString = QString("%1,%2,%3,%4").arg(newColor.red())
            .arg(newColor.green()).arg(newColor.blue()).arg(newColor.alpha());
    btn->setText(colorString);
    VisuMisc::setBackgroundColor(btn, newColor);
    return row;
}

QString VisuMisc::getValueString(int row, QString key, QTableWidget* table)
{
    QString value;
    if (VisuMisc::isColorProperty(key))
    {
        QPushButton* btn = static_cast<QPushButton*>(table->cellWidget(row, 1));
        value = btn->text();
    }
    else
    {
        value = table->item(row,1)->text();
    }
    return value;
}

