#include "wysiwyg/visupropertieshelper.h"
#include "visumisc.h"

#include <QFontDatabase>

const char* VisuPropertiesHelper::PROP_COLOR = "color";
const char* VisuPropertiesHelper::PROP_ROW = "row";
const char* VisuPropertiesHelper::PROP_KEY = "key";
const char* VisuPropertiesHelper::PROP_TYPE = "type";

double VisuPropertiesHelper::sliderToDouble(int slider)
{
    return (double)slider / SLIDER_FACTOR;
}

int VisuPropertiesHelper::doubleToSlider(double value)
{
    return (int)(value * SLIDER_FACTOR);
}

void VisuPropertiesHelper::setupTableWidget(
                                QTableWidget* table,
                                QWidget* widget,
                                QWidget* object,
                                const char* signal,
                                const char* slot,
                                QString key,
                                QString type,
                                int row)
{
    widget->setProperty(VisuPropertiesHelper::PROP_KEY, key);
    widget->setProperty(VisuPropertiesHelper::PROP_TYPE, type);
    widget->setProperty(VisuPropertiesHelper::PROP_ROW, row);
    table->setCellWidget(row, 1, widget);

    if (object != nullptr && slot != nullptr)
    {
        QObject::connect(widget, signal, object, slot);
    }
}

QComboBox* VisuPropertiesHelper::setupEnumWidget(VisuPropertyMeta meta, QString value)
{
    QComboBox* box = new QComboBox();
    box->insertItems(0, meta.getEnumOptions());
    box->setCurrentIndex(value.toInt());
    return box;
}

QComboBox* VisuPropertiesHelper::setupFontWidget(VisuPropertyMeta meta, QString value)
{
    QComboBox* box = new QComboBox();
    QFontDatabase db;
    box->insertItems(0, db.families());
    box->setCurrentText(value);
    return box;
}

QComboBox* VisuPropertiesHelper::setupBoolWidget(VisuPropertyMeta meta, QString value)
{
    QComboBox* box = new QComboBox();
    box->addItem("No", QVariant(0));
    box->addItem("Yes", QVariant(1));
    box->setCurrentIndex(value.toInt());
    return box;
}

QComboBox* VisuPropertiesHelper::setupSignalsWidget(VisuPropertyMeta meta, QString value)
{
    QComboBox* box = new QComboBox();
    int selected = 0;
    int intValue = value.toInt();
    QVector<QPointer<VisuSignal>> signalList = VisuConfiguration::get()->getSignals();
    for (VisuSignal* visuSignal : signalList)
    {
        if (visuSignal != nullptr)
        {
            box->addItem(visuSignal->getName(), QVariant(visuSignal->getId()));
            if (visuSignal->getId() == intValue)
            {
                selected = box->count()-1;
            }
        }
    }

    return box;
}

QComboBox* VisuPropertiesHelper::setupImagesWidget(VisuPropertyMeta meta, QString value)
{
    QComboBox* box = new QComboBox();
    box->addItem("Use color", QVariant(-1));

    QVector<QPointer<StaticImage> > list = VisuConfiguration::get()->getListOf<StaticImage>();
    int selected = 0;
    int intValue = value.toInt();
    for (StaticImage* image : list)
    {
        if (image != nullptr)
        {
            box->addItem(image->getName(), QVariant(image->getId()));

            if (image->getId() == intValue)
            {
                selected = box->count()-1;
            }
        }
    }
    box->setCurrentIndex(selected);
    return box;
}

QPushButton* VisuPropertiesHelper::setupColorWidget(VisuPropertyMeta meta, QString value, QColor color)
{
    QPushButton* btn = new QPushButton(value);
    VisuMisc::setBackgroundColor(btn, color);
    btn->setProperty(VisuPropertiesHelper::PROP_COLOR, color);
    return btn;
}


QSpinBox* VisuPropertiesHelper::setupIntWidget(VisuPropertyMeta meta, QString value)
{
    QSpinBox* spinbox = new QSpinBox();
    spinbox->setMinimum(meta.min);
    spinbox->setMaximum(meta.max);
    spinbox->setValue(value.toInt());
    return spinbox;
}

QSlider* VisuPropertiesHelper::setupSliderWidget(VisuPropertyMeta meta, QString value)
{
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(VisuPropertiesHelper::doubleToSlider(meta.min));
    slider->setMaximum(VisuPropertiesHelper::doubleToSlider(meta.max));
    slider->setValue(VisuPropertiesHelper::doubleToSlider(value.toDouble()));
    return slider;
}

QLineEdit* VisuPropertiesHelper::setupDefaultWidget(VisuPropertyMeta meta, QString value)
{
    QLineEdit* edit = new QLineEdit(value);

    if (meta.type == VisuPropertyMeta::TYPE_FLOAT)
    {
        QValidator *validator = new QDoubleValidator(meta.min, meta.max, 3);
        edit->setValidator(validator);
    }
    return edit;
}



void VisuPropertiesHelper::updateTable(QTableWidget* table,
                           QMap<QString, QString> properties,
                           QMap<QString, VisuPropertyMeta> metaProperties,
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
        if (metaProperties.contains(key))
        {
            meta = metaProperties[key];
        }

        QWidget* widget = nullptr;
        const char* widgetSignal = nullptr;

        if (meta.type == VisuPropertyMeta::TYPE_ENUM)
        {
            widget = VisuPropertiesHelper::setupEnumWidget(meta, value);
            widgetSignal = SIGNAL(currentIndexChanged(int));
        }
        else if (meta.type == VisuPropertyMeta::TYPE_FONT)
        {
            widget = VisuPropertiesHelper::setupFontWidget(meta, value);
            widgetSignal = SIGNAL(currentIndexChanged(int));
        }
        else if (meta.type == VisuPropertyMeta::TYPE_BOOL)
        {
            widget = VisuPropertiesHelper::setupBoolWidget(meta, value);
            widgetSignal = SIGNAL(currentIndexChanged(int));
        }
        else if (meta.type == VisuPropertyMeta::TYPE_SIGNAL)
        {
            widget = VisuPropertiesHelper::setupSignalsWidget(meta, value);
            widgetSignal = SIGNAL(currentIndexChanged(int));
        }
        else if (meta.type == VisuPropertyMeta::TYPE_IMAGE)
        {
            widget = VisuPropertiesHelper::setupImagesWidget(meta, value);
            widgetSignal = SIGNAL(currentIndexChanged(int));
        }
        else if (meta.type == VisuPropertyMeta::TYPE_COLOR)
        {
            widget = VisuPropertiesHelper::setupColorWidget(meta, value, VisuHelper::get<QColor>(key, properties));
            widgetSignal = SIGNAL(clicked());
        }
        else if (meta.type == VisuPropertyMeta::TYPE_READ_ONLY)
        {
            QTableWidgetItem* item = new QTableWidgetItem(value);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            table->setItem(row, 1, item);
        }
        else if (meta.type == VisuPropertyMeta::TYPE_INT)
        {
            widget = VisuPropertiesHelper::setupIntWidget(meta, value);
            widgetSignal = SIGNAL(valueChanged(int));
        }
        else if (meta.type == VisuPropertyMeta::TYPE_SLIDER)
        {
            widget = VisuPropertiesHelper::setupSliderWidget(meta, value);
            widgetSignal = SIGNAL(valueChanged(int));
        }
        else
        {
            widget = VisuPropertiesHelper::setupDefaultWidget(meta, value);
            widgetSignal = SIGNAL(editingFinished());
        }

        if (widget != nullptr)
        {
            VisuPropertiesHelper::setupTableWidget(table,
                                                   widget,
                                                   object,
                                                   widgetSignal,
                                                   slot,
                                                   key,
                                                   meta.type,
                                                   row);
        }

        ++row;
    }
}

QString VisuPropertiesHelper::getValueString(QTableWidget* table, int row)
{
    QString value;
    QComboBox* box;
    QLineEdit* edit;
    QPushButton* button;
    QSpinBox* spinbox;
    QSlider* slider;

    if ( (box = qobject_cast<QComboBox*>(table->cellWidget(row, 1))) != nullptr)
    {
        if (box->property(VisuPropertiesHelper::PROP_TYPE).toString() == VisuPropertyMeta::TYPE_FONT)
        {
            value = box->currentText();
        }
        else if (box->property(VisuPropertiesHelper::PROP_TYPE).toString() == VisuPropertyMeta::TYPE_ENUM)
        {
            value = QString("%1").arg(box->currentIndex());
        }
        else
        {
            value = QString("%1").arg(box->currentData().toInt());
        }
    }
    else if ( (spinbox = qobject_cast<QSpinBox*>(table->cellWidget(row, 1))) != nullptr)
    {
        value = QString("%1").arg(spinbox->value());
    }
    else if ( (edit = qobject_cast<QLineEdit*>(table->cellWidget(row, 1))) != nullptr )
    {
        value = edit->text();
    }
    else if ( (button = qobject_cast<QPushButton*>(table->cellWidget(row, 1))) != nullptr )
    {
        value = button->text();
    }
    else if (  (slider = qobject_cast<QSlider*>(table->cellWidget(row, 1))) != nullptr )
    {
        value = QString("%1").arg(VisuPropertiesHelper::sliderToDouble(slider->value()));
    }
    else
    {
        value = table->item(row,1)->text();
    }
    return value;
}

int VisuPropertiesHelper::updateWidgetProperty(QObject* sender, QWidget* parent)
{
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
