#include "visumisc.h"

#include <QPalette>
#include <QPushButton>
#include <QColorDialog>
#include <QTextStream>
#include <QComboBox>
#include <QLineEdit>
#include "visuhelper.h"

const char* VisuMisc::PROP_COLOR = "color";
const char* VisuMisc::PROP_ROW = "row";
const char* VisuMisc::PROP_KEY = "key";
const char* VisuMisc::PROP_X = "x";
const char* VisuMisc::PROP_Y = "y";
const char* VisuMisc::PROP_SIGNAL_ID = "signalId";
const char* VisuMisc::PROP_ID = "id";

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

void VisuMisc::setupTableWidget(QWidget* widget,
                                QTableWidget* table,
                                QWidget* object,
                                const char* slot,
                                QString key,
                                int row)
{
    widget->setProperty(VisuMisc::PROP_KEY, key);
    widget->setProperty(VisuMisc::PROP_ROW, row);
    table->setCellWidget(row, 1, widget);

    if (object != nullptr && slot != nullptr)
    {
        QObject::connect(widget, SIGNAL(currentIndexChanged(int)), object, slot);
    }
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

        if (meta.type == VisuPropertyMeta::TYPE_ENUM)
        {
            QComboBox* box = new QComboBox();

            box->insertItems(0, meta.getEnumOptions());
            box->setCurrentIndex(value.toInt());

            VisuMisc::setupTableWidget(box, table, object, slot, key, row);
        }
        else if (meta.type == VisuPropertyMeta::TYPE_COLOR)
        {
            QColor color = VisuHelper::get<QColor>(key, properties);
            QPushButton* btn = new QPushButton(i.value());
            VisuMisc::setBackgroundColor(btn, color);
            btn->setProperty(VisuMisc::PROP_COLOR, color);

            VisuMisc::setupTableWidget(btn, table, nullptr, nullptr, key, row);
            if (object != nullptr && slot != nullptr)
            {
                QObject::connect(btn, SIGNAL(clicked()), object, slot);
            }
        }
        else if (meta.type == VisuPropertyMeta::TYPE_BOOL)
        {
            QComboBox* box = new QComboBox();
            box->insertItem(0, "No");
            box->insertItem(1, "Yes");
            box->setCurrentIndex(value.toInt());

            VisuMisc::setupTableWidget(box, table, object, slot, key, row);
        }
        else if (meta.type == VisuPropertyMeta::TYPE_SIGNAL)
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

                VisuMisc::setupTableWidget(box, table, object, slot, key, row);
            }
        }
        else if (meta.type == VisuPropertyMeta::TYPE_READ_ONLY)
        {
            QTableWidgetItem* item = new QTableWidgetItem(value);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            table->setItem(row, 1, item);
        }
        else
        {
            //table->setItem(row, 1, new QTableWidgetItem(value));

            QLineEdit* edit = new QLineEdit(value);
            VisuMisc::setupTableWidget(edit, table, nullptr, nullptr, key, row);

            if (meta.type == VisuPropertyMeta::TYPE_INT)
            {
                QValidator *validator = new QIntValidator(meta.min, meta.max);
                edit->setValidator(validator);
            }
            else if (meta.type == VisuPropertyMeta::TYPE_FLOAT)
            {
                QValidator *validator = new QDoubleValidator(meta.min, meta.max, 3);
                edit->setValidator(validator);
            }

            if (object != nullptr && slot != nullptr)
            {
                QObject::connect(edit, SIGNAL(editingFinished()), object, slot);
            }
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

    QPushButton* btn;

    if ( (btn = qobject_cast<QPushButton*>(sender)) != nullptr )
    {
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

    return row;
}

QString VisuMisc::getValueString(int row, QString key, QTableWidget* table)
{
    QString value;

    QComboBox* box;
    QLineEdit* edit;
    QPushButton* button;

    if ( (box = qobject_cast<QComboBox*>(table->cellWidget(row, 1))) != nullptr)
    {
        value = QString("%1").arg(box->currentIndex());
    }
    else if ( (edit = qobject_cast<QLineEdit*>(table->cellWidget(row, 1))) != nullptr )
    {
        value = edit->text();
    }
    else if ( (button = qobject_cast<QPushButton*>(table->cellWidget(row, 1))) != nullptr )
    {
        value = button->text();
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
