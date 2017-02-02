#include "visumisc.h"

#include <QPalette>
#include <QPushButton>
#include <QColorDialog>
#include <QTextStream>
#include <QComboBox>
#include "visuhelper.h"

const char* VisuMisc::PROP_COLOR = "color";
const char* VisuMisc::PROP_ROW = "row";
const char* VisuMisc::PROP_KEY = "key";
const char* VisuMisc::PROP_X = "x";
const char* VisuMisc::PROP_Y = "y";
const char* VisuMisc::PROP_SIGNAL_ID = "signalId";
const char* VisuMisc::PROP_ID = "id";

bool VisuMisc::isColorProperty(QString property)
{
    return property.contains(VisuMisc::PROP_COLOR);
}

bool VisuMisc::isSignalIdProperty(QString property)
{
    return property.contains(VisuMisc::PROP_SIGNAL_ID);
}

bool VisuMisc::isIdProperty(QString property)
{
    return property == VisuMisc::PROP_ID;
}

void VisuMisc::setBackgroundColor(QWidget* widget, QColor color)
{
    QString stylesheet = QString("background-color: rgb(%1, %2, %3);")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    widget->setStyleSheet(stylesheet);
}

void VisuMisc::updateTable(QTableWidget* table,
                           QMap<QString, QString> properties,
                           QMap<QString, VisuPropertyMeta>* metaProperties,
                           QVector<QPointer<VisuSignal>>* signalList,
                           QWidget* object,
                           const char* slot)
{
    table->clearContents();
    table->setRowCount(properties.size());
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList{"Property", "Value"});

    int row = 0;
    for (auto i = properties.begin(); i != properties.end(); ++i)
    {
        QString key = i.key();
        QString value = i.value();
        table->setItem(row, 0, new QTableWidgetItem(key));

        VisuPropertyMeta meta;
        if (metaProperties != nullptr && metaProperties->contains(key))
        {
            meta = (*metaProperties)[key];
        }

        if (meta.type == "enum")
        {
            QComboBox* box = new QComboBox();

            box->insertItems(0, meta.extra.split(","));
qDebug("Current value: %d", value.toInt());
            box->setCurrentIndex(value.toInt());
            box->setProperty(VisuMisc::PROP_KEY, key);
            box->setProperty(VisuMisc::PROP_ROW, row);
            table->setCellWidget(row, 1, box);

            if (object != nullptr && slot != nullptr)
            {
                QObject::connect(box, SIGNAL(currentIndexChanged(int)), object, slot);
            }
        }
        if (VisuMisc::isColorProperty(key))
        {
            QColor color = VisuHelper::get<QColor>(key, properties);
            QPushButton* btn = new QPushButton(i.value());
            btn->setProperty(VisuMisc::PROP_COLOR, color);
            btn->setProperty(VisuMisc::PROP_KEY, key);
            btn->setProperty(VisuMisc::PROP_ROW, row);
            VisuMisc::setBackgroundColor(btn, color);
            table->setCellWidget(row, 1, btn);
            if (object != nullptr && slot != nullptr)
            {
                QObject::connect(btn, SIGNAL(clicked()), object, slot);
            }
        }
        else if (VisuMisc::isSignalIdProperty(key))
        {
            if (signalList != nullptr)
            {
                QComboBox* box = new QComboBox();
                for (VisuSignal* visuSignal : *signalList)
                {
                    if (visuSignal != nullptr)
                    {
                        box->insertItem(visuSignal->getId(), visuSignal->getName());
                    }
                }
                box->setCurrentIndex(value.toInt());
                box->setProperty(VisuMisc::PROP_KEY, key);
                box->setProperty(VisuMisc::PROP_ROW, row);
                table->setCellWidget(row, 1, box);

                if (object != nullptr && slot != nullptr)
                {
                    QObject::connect(box, SIGNAL(currentIndexChanged(int)), object, slot);
                }
            }
        }
        else if (VisuMisc::isIdProperty(key))
        {
            // delete row with key
            QTableWidgetItem* item = new QTableWidgetItem(value);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            table->setItem(row, 1, item);
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

int VisuMisc::updateWidgetProperty(QObject* sender, QWidget* parent)
{
    QString key = sender->property(VisuMisc::PROP_KEY).toString();
    int row = sender->property(VisuMisc::PROP_ROW).toInt();

    if (VisuMisc::isColorProperty(key))
    {
        QPushButton* btn = static_cast<QPushButton*>(sender);
        QColor oldColor = btn->property(VisuMisc::PROP_COLOR).value<QColor>();
        QColor newColor = QColorDialog::getColor(oldColor,
                                                 parent,
                                                 QObject::tr("Set color"),
                                                 QColorDialog::ShowAlphaChannel);

        if (newColor.isValid())
        {
            QString colorString = QString("%1,%2,%3,%4").arg(newColor.red())
                    .arg(newColor.green()).arg(newColor.blue()).arg(newColor.alpha());
            btn->setText(colorString);
            VisuMisc::setBackgroundColor(btn, newColor);
        }
    }
    else if (VisuMisc::isSignalIdProperty(key))
    {
        //QComboBox* box = static_cast<QComboBox*>(sender);
    }

    return row;
}

QString VisuMisc::getValueString(int row, QString key, QTableWidget* table)
{
    QString value;

    QComboBox* box = qobject_cast<QComboBox*>(table->cellWidget(row, 1));
    if (box != nullptr)
    {
        qDebug("New combo value: %d", box->currentIndex());
        value = QString("%1").arg(box->currentIndex());
    }
    else if (VisuMisc::isColorProperty(key))
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
