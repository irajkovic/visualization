#include "wysiwyg/visupropertieshelper.h"
#include "visumisc.h"

const char* VisuPropertiesHelper::PROP_COLOR = "color";
const char* VisuPropertiesHelper::PROP_ROW = "row";
const char* VisuPropertiesHelper::PROP_KEY = "key";
const char* VisuPropertiesHelper::PROP_X = "x";
const char* VisuPropertiesHelper::PROP_Y = "y";
const char* VisuPropertiesHelper::PROP_SIGNAL_ID = "signalId";
const char* VisuPropertiesHelper::PROP_ID = "id";

void VisuPropertiesHelper::setupTableWidget(QWidget* widget,
                                QTableWidget* table,
                                QWidget* object,
                                const char* slot,
                                QString key,
                                int row)
{
    widget->setProperty(VisuPropertiesHelper::PROP_KEY, key);
    widget->setProperty(VisuPropertiesHelper::PROP_ROW, row);
    table->setCellWidget(row, 1, widget);

    if (object != nullptr && slot != nullptr)
    {
        QObject::connect(widget, SIGNAL(currentIndexChanged(int)), object, slot);
    }
}

void VisuPropertiesHelper::updateTable(QTableWidget* table,
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

            VisuPropertiesHelper::setupTableWidget(box, table, object, slot, key, row);
        }
        else if (meta.type == VisuPropertyMeta::TYPE_COLOR)
        {
            QColor color = VisuHelper::get<QColor>(key, properties);
            QPushButton* btn = new QPushButton(i.value());
            VisuMisc::setBackgroundColor(btn, color);
            btn->setProperty(VisuPropertiesHelper::PROP_COLOR, color);

            VisuPropertiesHelper::setupTableWidget(btn, table, nullptr, nullptr, key, row);
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

            VisuPropertiesHelper::setupTableWidget(box, table, object, slot, key, row);
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

                VisuPropertiesHelper::setupTableWidget(box, table, object, slot, key, row);
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
            VisuPropertiesHelper::setupTableWidget(edit, table, nullptr, nullptr, key, row);

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

QString VisuPropertiesHelper::getValueString(int row, QString key, QTableWidget* table)
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

int VisuPropertiesHelper::updateWidgetProperty(QObject* sender, QWidget* parent)
{
    QString key = sender->property(VisuPropertiesHelper::PROP_KEY).toString();
    int row = sender->property(VisuPropertiesHelper::PROP_ROW).toInt();

    QPushButton* btn;

    if ( (btn = qobject_cast<QPushButton*>(sender)) != nullptr )
    {
        QColor oldColor = btn->property(VisuPropertiesHelper::PROP_COLOR).value<QColor>();
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
