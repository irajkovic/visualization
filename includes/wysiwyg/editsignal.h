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
    EditSignal(QPointer<VisuSignal> visuSignal = nullptr);

signals:
    void signalAdded(VisuSignal*,bool);

public slots:
    void addSignal();
    void cellUpdated(int row, int col);

private:
    QMap<QString, QString> mProperties;
    QPointer<QTableWidget> mTable;
    bool mNewSignal;
    QPointer<VisuSignal> mSignal;
};

#endif // EDITSIGNAL_H
