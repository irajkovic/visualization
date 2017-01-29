#ifndef EDITSIGNAL_H
#define EDITSIGNAL_H

#include <QPointer>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include "visumisc.h"
#include "visuconfigloader.h"
#include "visusignal.h"
#include <QTableWidget>
#include <QMap>

class EditSignal : public QWidget
{
    Q_OBJECT

public:
    EditSignal(QWidget* parent, QPointer<VisuSignal> visuSignal = nullptr) : QWidget(parent)
    {
        setWindowFlags(Qt::Dialog);
        setup(visuSignal);
    }

signals:
    void signalAdded(QPointer<VisuSignal>,bool);

public slots:
    void addSignal();
    void cellUpdated(int row, int col);

private:
    QMap<QString, QString> mProperties;
    QPointer<QTableWidget> mTable;
    bool mNewSignal;
    QPointer<VisuSignal> mSignal;

    void setup(QPointer<VisuSignal> visuSignal);
};

#endif // EDITSIGNAL_H
