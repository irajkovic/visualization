#ifndef EDITSIGNAL_H
#define EDITSIGNAL_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>

class EditSignal : public QWidget
{
public:
    EditSignal(QWidget* parent = 0)
    {
        QWidget* centerWidget = new QWidget();
        QLayout* vlayout = new QVBoxLayout();
        centerWidget->setLayout(vlayout);

        QWidget* form = new QWidget();

        QWidget* buttons = new QWidget();
        QLayout* buttonsLayout = new QHBoxLayout();
        buttons->setLayout(buttonsLayout);

        QPushButton* saveButton = new QPushButton(tr("&Save"));
        buttonsLayout->addWidget(saveButton);

        QPushButton* cancelButton = new QPushButton(tr("&Cancel"));
        buttonsLayout->addWidget(cancelButton);

        buttons->setMaximumHeight(saveButton->height());
        vlayout->addWidget(buttons);

        centerWidget->setGeometry(100, 100, 600, 600);
        centerWidget->show();
    }
};

#endif // EDITSIGNAL_H
